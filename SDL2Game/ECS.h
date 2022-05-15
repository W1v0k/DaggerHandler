#pragma once

//ECS - Entity Component System

#include <iostream> //input output stream
#include <vector> //identic cu array dinamic doar ca au abilitatea sa se redimensioneze singuri cand introducem sau extragem un element
#include <memory> //pentru utilizarea memoriei dinamice
#include <algorithm> //sortari, numarari, manipulari, cautari, etc.
#include <bitset> //emuleaza un vector de elemente bool, dar optimizata pentru alocarea spatiului(stocheaza bits, elemente cu doar 2 valori posibile)
#include <array> //vectori care au o dimensiune fixata

//spunem compilatorului ca avem clasele acestea, doar ca nu le-am definit inca
class Component; //pos,physics,AI,input handeling, pentru toate acestea avem nevoie de un id
class Entity;
class Manager;

//using este folosit ca un typedef in situatia asta.
using ComponentID = std::size_t; //size_t unsigned integer care stocheaza dimensiunea maxima a unui posibil obiect de orice tip(inclusiv arrays)
using Group = std::size_t;

inline ComponentID getNewComponentTypeID() //in loc sa se apeleze functia, se inlocuieste cu codul acesteia unde avem nevoie
{ //ComponentID este tipul returnat de functie
	static ComponentID lastID = 0u;//u la sfarsit pentru unsigned
	return lastID++;
	//cand apelam functia, prima apelare va returna 0, a doua 1, a treia 2, samd.(folosim lastID++, si nu ++lastID)
}

template <typename T> inline ComponentID getComponentTypeID() noexcept //Explicatia1+Explicatia2
{
	static_assert(std::is_base_of<Component, T>::value, "");//is_base_of returneaza un boolean; template<class Base, class Derived> struct is_base_of; Daca Derived este derivata din Base returneaza true.
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
};

//verificam daca o componenta are o entitate atasata
constexpr std::size_t maxComponents = 32; //Explicatia3
constexpr std::size_t maxGroups = 32;//pentru render layers si collision layers

using ComponentBitSet = std::bitset<maxComponents>; //daca avem nevoie sa aflam daca o entitate are o selectie de componente, vom putea compara daca aceasta are sau nu componente.
//Asadar, generam un bitset, il pasam mai departe si daca corespunde semnaturii entitatii stim ca avem toate componentele necesare.
using GroupBitSet = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>; //ComponentArray va fi un array de elemente Component*, care va avea dimensiunea maxComponents

class Component //clasa abstracta
{
public:
	Entity* entity; //are o referinta la proprietarul lui
	
	virtual void init() {} //Explicatia4
	virtual void update() {} //void tipul returnat
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	Manager& manager;
	bool active = true; //vom contoriza daca entitatea este activa sau nu; daca este fals, putem sterge entitatea din joc
	std::vector<std::unique_ptr<Component>> components; //Explicatia5 ;o lista a tuturor componentelor pe care o entitate o detine 
	
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:
	Entity(Manager& mManager) : manager(mManager) {}
	void update()
	{
		//entitatea parcurge o bucla prin fiecare componenta a sa, si apeleaza fiecare functie update si draw a lor
		for (auto& c : components) c->update(); //asemanator cu python, for component in components
	}

	void draw()
	{
		for (auto& c : components) c->draw();
	}

	bool isActive() const { return active; }
	void destroy() { active = false; } //deoarece componenta are o referinta catre proprietar(entitate) putem apela functia destroy a entitatii de la una din componente

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup]; //returneaza true sau false
	}

	void addGroup(Group mGroup);
	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	//vrem ca entitatea sa vada daca are componente, ne folosim de bitset

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()]; //cand creem entitatea si vrem sa vedem daca are componente,trimitem pozitia componentului si putem verifica daca e adevarat sau fals, deci daca avem sau nu acea componenta atasata
	}

	template <typename T, typename... TArgs> //adaugarea unei componente la o entitate
	T& addComponent(TArgs&&... mArgs) //Explicatia6
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr)); //Explicatia7

		componentArray[getComponentTypeID<T>()] = c; //cand adaugam o componenta la o pozitie, va fi pusa mereu in aceasi pozitie si in vector
		componentBitSet[getComponentTypeID<T>()] = true; //am adaugat o componenta, deci exista

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]); //ne folosim de pointerul asta ca sa pozitionam elementele in vector
		return * static_cast<T*>(ptr); //se face un cast la tipul de data T*
	}	
	//Utilizare: gameobject.getComponent<PositionComponent>().setXpos(25);
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;//lista tuturor entitatilor
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	void wipe()
	{
		for (auto& e : entities)
		{
			e->destroy();
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));

		entities.clear();
		for (auto& e : groupedEntities) e.clear();
	}

	void update()//ne folosim de manager sa facem update la toate entitatile
	{
		for (auto& e : entities)e->update();
	}
	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	//la fiecare frame, vrem ca managerul sa parcurga entitatile si sa le stearga pe cele care nu sunt acolo
	void refresh()
	{
		for (auto i(0u); i < maxGroups; ++i)//auto i(0u) porneste de la 0 unsigned
		{
			auto& vectors(groupedEntities[i]);
			vectors.erase(
				std::remove_if(std::begin(vectors), std::end(vectors), [i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i); //returnez stanga, daca nu, returnez dreapta
					}),
				std::end(vectors));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities)); //Explicatia8 , sterge entitatile daca nu sunt active
	}

	void addToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	//void delGroup(Group mGroup)
	//{
	//	std::vector<Entity*> entities = groupedEntities[mGroup];

	//	for (auto& e : entities)
	//	{
	//		e->destroy();
	//	}

	//	groupedEntities[mGroup].clear();
	//	refresh();
	//}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this); //referinta this pentru manager
		std::unique_ptr<Entity> uPtr{ e }; //initializarea unique_ptr cu e
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};


/* 
===EXPLICATIA1===
template <parameter - list> declarare
parameter-list poate fi: non-tip template, de tip template, the tip template template
https://www.youtube.com/watch?v=I-hZkUa9mIs
Templateurile sunt ca un fel de macro. Dupa definirea templateului, indemni compilatorul sa scrie cod pentru tine, dupa anumite reguli pe care le oferim.
Cand scriem o functie si folosim un template in functie, creem un fel de blueprint, astfel incat, in momentul in care apelam functia sa putem oferi cativa parametri
care sa determine care cod se va inlocui in template.

Utilizari ale templateului, ex:
Sa creem o functie care va rula acelasi cod, dar va primi parametri diferiti.
>EX1(template pt functie):
#include <iostream>
#include <string>

void Print(int value)
{
std::cout<<value<<std::endl;
}

void Print(std::string value)
{
	std::cout<<value<<std::endl;
}

void Print(float value)
{
	std::cout<<value<<std::endl;
}

int main()
{
	Print(5);
	Print("Hello");
	Print(5.5f);
	std::cin.get();
}
Ajungem sa definim 3 supraincarcari ale aceleasi functii, dar putem utiliza templateurile ca sa evitam duplicarea codului.Daca cumva nu mai vrem sa utilizam
functia cout pentru print, si sa folosim printf, ar trebui sa facem modificarea in toate functiile, cu un template trebuie facuta modificarea doar odata.

Utilizam acum templates:
#include <iostream>
#include <string>

template<typename T> //nu este exact un cod real, ci va fi evaluat la compilare, creat cand este apelata functia(depinzand de cum chemam functia, cu ce parametri)
void Print(T value) //T tine locul oricarui tip de data l-am transmite: int, string, float,etc.
{
std::cout<<value<<std::endl;
}

int main()
{
Print(5);
Print("Hello");
Print(5.5f);
std::cin.get();
}

Putem rescrie mainul:
{
Print<int>(5); //pentru a specifica explicit tipul de data al parametrului transmis(typename T va prelua int). Se poate folosi si class T, fiind sinonime
//se evita deobicei class T fiindca nu se refera la utilizarea unui type class
}
Daca nu am apela niciodata functia Print, codul acesteia practic nu ar exista, acesta fiind creat doar la apelare. Acest lucru se poate demonstra daca facem intentionat
o eroare de sintaxa, scriem valu in loc de value. Compilatorul nu va atentiona programatorul de aceasta greseala => va compila cu cod de eroare 0(depinde si de compilator, VS nu va atentiona).
Ideea de baza: folosim templateuri, specificam cum sa se creeze apoi metode pe baza lor, utilizam metodele create pentru situatii specifice.

>EX2(template pentru clasa):
#include <iostream>
#include <string>

template <int N>
class Array
{
private:
int m_Array[N]; //deoarece creem un array depus pe stiva, avem nevoie de dimensiunea lui la compilare, vom utiliza asadar template
public:
int GetSize() const {return N;}
//const dupa numele functiei face ca functia in sine sa fie const. Definirea asta se poate utiliza doar la functii mebre ale unei clase sau ale unui struct.
//o functie const inseamna : nu poate apela alte functii non-constante membre, nu poate schimba variabile membre, poate fi apelat si de obiecte constante, si de obiecte non-constante, trebuie sa fie o functie membra a unei clase.
};

int main()
{
	Array<5> array; //se creaza o versiune a templateului de mai sus, adica o clasa, unde N se inlocuieste cu 5.
	std::cout<<array.GetSize() <<std::endl; //printeaza 5
}

Alta versiune:
template <typename T, int N>
class Array
{
private: 
T m_Array[N];
public:
int GetSize() const{return N;}
};
int main()
{
	Array<std::string,50>array;
}

Templateurile sunt ca o metaprogramare(=tehnica de programare in care programul poate fi desemnat sa citeaza, genereze, analizeze si sa transforme alte programe, sau de a se modifica pe el insusi in timpul rularii)
Unele companii interzic folosirea templateurilor deoarece pot ajunge la un moment dat sa fie foarte complexe si sa fie foarte greu de facut debugg in cazul in care ceva nu functioneaza corect.


===Explicatia2===
Operatorul noexcept performeaza o verificare la compilare care returneaza true daca o expresie este declarata sa nu arunce nici o exceptie.

===Eplicatia3===
constexpr este utilizata ca inline, pentru eficientizarea performantei programului unde calculele se realizeaza in timpul compilarii si nu la rulare.
Ex:
constexpr int product(int x, int y)
{return (x*y);}
int main(){const int x = product(10,20); cout<<x; return 0;}

===Explicatia4===
Virtual se foloseste ca sa poata fi suprascrisa functia. A virtual function is a member function which is declared within a base class and is re-defined(Overridden) by a derived class.
They are mainly used to achieve Runtime polymorphism.
Rules:
Virtual functions cannot be static.
Virtual functions should be accessed using pointer or reference of base class type to achieve run time polymorphism.
The prototype of virtual functions should be the same in the base as well as derived class.

===Explicatia5===
An object when described using auto_ptr class it stores a pointer to a single allocated object which ensures that when it goes out of scope, the object it points to must get automatically destroyed. 
It is based on exclusive ownership model i.e. two pointers of the same type can’t point to the same resource at the same time.

unique_ptr was developed as a replacement for auto_ptr
unique_ptr is a new facility with similar functionality, but with improved security (no fake copy assignments), added features (deleters) and support for arrays. It is a container for raw pointers. 
It explicitly prevents copying of its contained pointer as would happen with normal assignment i.e. it allows exactly one owner of the underlying pointer.

===Explicatia6===
A template parameter pack is a template parameter that accepts zero or more template arguments (non-types, types, or templates). 
A function parameter pack is a function parameter that accepts zero or more function arguments.

template<class ... Types> void f(Types ... args);
f();       // OK: args contains no arguments
f(1);      // OK: args contains one argument: int
f(2, 1.0); // OK: args contains two arguments: int and double


C++11 adds a new non-const reference type called an rvalue reference, identified by T&&. 
This refers to temporaries that are permitted to be modified after they are initialized, for the purpose of allowing "move semantics".
&& is new in C++11. int&& a means "a" is an r-value reference. && is normally only used to declare a parameter of a function. 
And it only takes a r-value expression.


lvalue (1)
template <class T> T&& forward (typename remove_reference<T>::type& arg) noexcept;
rvalue (2)
template <class T> T&& forward (typename remove_reference<T>::type&& arg) noexcept;

Forward argument
Returns an rvalue reference to arg if arg is not an lvalue reference.
If arg is an lvalue reference, the function returns arg without modifying its type.
The need for this function stems from the fact that all named values (such as function parameters) always evaluate as lvalues (even those declared as rvalue references), 
and this poses difficulties in preserving potential move semantics on template functions that forward arguments to other functions.
Both signatures return the same as:
static_cast<decltype(arg)&&>(arg)

===Explicatia7===
emplace_back
Appends a new element to the end of the container. The element is constructed through std::allocator_traits::construct, which typically uses placement-new to construct the element in-place at the location provided by the container.
The arguments args... are forwarded to the constructor as std::forward<Args>(args)... .
Return value: a reference to the inserted element.


A unique_ptr does not share its pointer. It cannot be copied to another unique_ptr, passed by value to a function,or
used in any C++ STD Library algorithm that requires copies to be made. A unique_ptr can only be moved.
This means that the ownership of the memory resource is transferred to another unique_ptr and the original unique_ptr no longer owns it. 
We recommend that you restrict an object to one owner, because multiple ownership adds complexity to the program logic.
Moving the unique_ptr nulls it.

===Explicatia8===
Removes from the vector either a single element (position) or a range of elements ([first,last)).

This effectively reduces the container size by the number of elements removed, which are destroyed.

Because vectors use an array as their underlying storage, erasing elements in positions other than the vector end causes the container to relocate all the elements after the segment erased to their new positions. 
This is generally an inefficient operation compared to the one performed for the same operation by other kinds of sequence containers (such as list or forward_list).
*/
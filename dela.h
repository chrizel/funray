/*funray - yet another raytracer
opyright (C) 2008  Christian Zeller (chrizel@gmail.com) and
                    Simon Goller (neosam@gmail.com).

This program is free software; you can redistribute it and/or modify 
it under the terms of the GNU General Public License as published 
by the Free Software Foundation; either version 3 of the License, 
or (at your option) any later version.

This program is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License along 
with this program; if not, see <http://www.gnu.org/licenses/>. */

#ifndef DELA_H
#define DELA_H

#include <QByteArray>
#include <QDebug>
#include <QList>
#include <QString>

namespace dela {

class List;
class String;
class Number;

class Scriptable
{
public:
    virtual ~Scriptable() {};
    virtual QByteArray toString() {
	return "{Scriptable <0x" + QByteArray::number((int)this, 16) + ">}";
    };
};

// Generic type checking functions...
template<class T>
inline T * asType(Scriptable *s) { 
    return dynamic_cast<T *>(s); 
}

template<class T>
inline bool isType(Scriptable *s) { 
    return asType<T>(s) != 0; 
}

template <class T>
inline T* ensureType(Scriptable *s)
{
    T* t = asType<T>(s);
    if (t)
	return t;

    qDebug() << "dela::ensureType: Wrong type error";
    exit(1);
    return 0;
}

// Generic toString function with 0-check
inline QByteArray toString(Scriptable *s) 
{
    if (s)
	return s->toString();
    else
	return "nil";
}

class String : public Scriptable
{
public:
    String(QByteArray value) : value(value) {
	//qDebug() << "New string: " << value;
    };
    virtual ~String() {
	//qDebug() << "Release string: " << value;
    };

    virtual QByteArray toString() {
	return value;
    };

    QByteArray value;
};

class Number : public Scriptable
{
public:
    Number(double value) : value(value) {
	//qDebug() << "New number: " << value;
    };
    virtual ~Number() {
	//qDebug() << "Release number: " << value;
    };

    virtual QByteArray toString() {
	return QByteArray::number(value);
    };

    double value;
};

class List : public QList<Scriptable *>, public Scriptable
{
public:
    List() {
	//qDebug() << "New List.";
    };
    virtual ~List() {
	//qDebug() << "Release List.";
    };

    virtual QByteArray toString() {
	QByteArray result = "";
	for (List::iterator it = begin(); it != end(); it++) {
	    if (!result.isEmpty())
		result += " ";
	    result += (*it)->toString();
	}
	return "(" + result + ")";
    };
};

class Engine;

typedef Scriptable * (*Function)(Engine *, List *);

class Engine
{
private:
    QHash<QByteArray, Function> functions;
    QHash<QByteArray, Function> macros;
    QHash<QByteArray, Scriptable*> variables;

    List * autoreleasePool;
    QByteArray code;
    int pos;

    // Parsing functions
    Scriptable * recogToken(QByteArray str);
    List * parse();

public:
    Engine();
    virtual ~Engine();

    Scriptable * eval(Scriptable * code);
    Scriptable * eval(const QByteArray &string, bool getResult = false);
    Scriptable * evalFile(const QString &name, bool getResult = false);

    void addFunction(QByteArray name, Function f) { functions[name] = f; };
    void addMacro(QByteArray name, Function f) { macros[name] = f; };
    void setVariable(QByteArray name, Scriptable *s) { variables[name] = s; };
    void unsetVariable(QByteArray name) { variables.remove(name); };

    Scriptable* readProperty(List *params, const QByteArray &name, int index);
    inline double readNumberProp(List *params, const QByteArray &name, int index) {
	return ensureType<Number>(readProperty(params, name, index))->value;
    };
    inline double readNumberPropDef(List *params, const QByteArray &name, 
				    int index, double def) {
	Number *n = asType<Number>(readProperty(params, name, index));
	return n ? n->value : def;
    };

    template <class T>
    T autorelease(T s);

    template <class T>
    T undoAutorelease(T s);

    static void deleteScriptable(Scriptable *s);
};

}

#endif

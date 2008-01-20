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

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QString>

#include <ctype.h>
#include <iostream>

#include "dela.h"
#include "dela_builtins.h"

namespace dela {

Scriptable * Engine::recogToken(QByteArray str)
{
    bool ok = false;
    float v = str.toDouble(&ok);
    if (ok)
	return autorelease(new Number(v));
    else
	return autorelease(new String(str));
}

List * Engine::parse()
{
    List * result = autorelease(new List);

    QByteArray token;
    bool is_comment = false;
    while(pos < code.size()) {
	char c = code.at(pos);
	pos++;

	// Still inside comment?
	// Ignore until end of line
	if (is_comment) {
	    if (c == '\n')
		is_comment = false;
	    continue;
	}

	// Token finish?
	if ((isspace(c) || c == '(' || c == ')' || c == ';') && (token.size() > 0)) {
	    result->append(recogToken(token));
	    token = "";
	}

	if (isspace(c))	// Ignore whitespace
	    continue;
	else if (c == ';') { // Start of comment
	    is_comment = true;
	    continue;
	} else if (c == '(') // ( starts a new list
	    result->append(parse());
	else if (c == ')') // ) ends the current list
	    return result;
	else
	    token += c;
    }

    return result;
}

Engine::Engine()
    : autoreleasePool(0), code(0), pos(0)
{
    addBuiltins(this);
}

Engine::~Engine()
{
}

Scriptable * Engine::eval(Scriptable * code)
{
    List *list = asType<List>(code);
    if (list) {
	if (list->isEmpty())
	    return 0;

	Scriptable *s = (*list)[0];
	String *name  = asType<String>(s);
	if (name) {
	    if (macros.contains(name->value)) {
		// Call macro with unevaluated parameters...
		List::iterator it;
		List *params = autorelease(new List);
		for (it = list->begin() + 1; it != list->end(); it++)
		    params->append(*it);

		return eval(macros[name->value](this, params));
	    } else if (functions.contains(name->value)) {
		// Go through list, interpret parameters and execute function
		List::iterator it;
		List *params = autorelease(new List);
		for (it = list->begin() + 1; it != list->end(); it++)
		    params->append(eval(*it));

		return functions[name->value](this, params);
	    } else {
		qDebug() << "dela::Engine::eval error: Unknown function: " << name->value;
		exit(1);
	    }
	} else if ((list = asType<List>(s))) {
	    return eval(list);
	} else {
	    qDebug() << "dela::Engine::eval error: No string at beginning of list";
	    exit(1);
	}
    }

    String *string = asType<String>(code);
    if (string) {
	// It's a string!... Check if it begins with a $, then resolve variable
	// and return Scriptable*
	if (string->value.startsWith("$")) {
	    QByteArray name = string->value.mid(1);
	    if (variables.contains(name)) {
		return variables[name];
	    } else {
		qDebug() << "dela::Engine::eval error: Variable not defined: " << string->value;
		exit(1);
	    }
	}
    }

    return code;
}


Scriptable * Engine::eval(const QByteArray &string, bool getResult)
{
    autoreleasePool = new List;
    code = "(begin \n" + string + "\n)";
    pos  = 0;

    // parse and evaluate code, remove result from autorelease pool
    Scriptable *result = undoAutorelease(eval(parse()));

    // delete items in autorelease pool
    List::iterator it;
    for (it = autoreleasePool->begin(); it != autoreleasePool->end(); it++)
	delete *it;
    delete autoreleasePool;

    if (getResult) {
	// note: the caller from eval has to delete this
	// result object itself with deleteScriptable
	// after it's not needed anymore...
	return result;
    } else {
	deleteScriptable(result);
	return 0;
    }
}

Scriptable * Engine::evalFile(const QString &name, bool getResult) 
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	qDebug() << "dela::Engine::evalFile: Cannot open file " << name;
	return 0;
    }

    return eval(file.readAll(), getResult);
}

void Engine::deleteScriptable(Scriptable *s)
{
    if (s) {
	List *list = asType<List>(s);
	if (list) {
	    List::iterator it;
	    for (it = list->begin(); it != list->end(); it++)
		deleteScriptable(*it);
	}
    }

    delete s;
}

template <class T>
T Engine::autorelease(T s) 
{
    if (s && !autoreleasePool->contains(s)) {
	autoreleasePool->append(s);
    }
    return s;
}

template <class T>
T Engine::undoAutorelease(T s) 
{
    if (autoreleasePool->contains(s)) {
	List *list = asType<List>(s);
	if (list) {
	    List::iterator it;
	    for (it = list->begin(); it != list->end(); it++)
		undoAutorelease(*it);
	}

	autoreleasePool->removeAll(s);
    }

    return s;
}

Scriptable* Engine::readProperty(List *params, const QByteArray &name, int index)
{
    index++;
    List *list;
    for (List::iterator it = params->begin(); it != params->end(); it++) {
	list = ensureType<List>(*it);
	if (list->size() >= index + 1) {
	    String *var = ensureType<String>(list->at(0));
	    if (var->value == name)
		return eval(list->at(index));
	}
    }

    return 0;
}

}

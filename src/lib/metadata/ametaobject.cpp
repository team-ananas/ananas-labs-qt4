/****************************************************************************
** $Id: ametaobject.cpp,v 1.5 2008/12/20 21:17:49 leader Exp $
**
** Code file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Grigory Panov <gr1313 at mail.ru>, Yoshkar-Ola.
**
** This file is part of the Library of the Ananas
** automation accounting system.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.leaderit.ru/page=ananas or email sales@leaderit.ru
** See http://www.leaderit.ru/gpl/ for GPL licensing information.
**
** Contact org@leaderit.ru if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include <qdatetime.h>
//#include <qobject.h>
#include <qfile.h>
#include <qstringlist.h>
#include <QTextStream>
#include <QDomDocument>
#include <QMutex>

#include "ametaobject.h"
#include "ametaobjectgroup.h"

/*!
 * \class AMetaObject
 *
 */
/*!
 *\en
 *	Metadata object for metadata script access.
 *\_en
 *\ru
 *	\brief Объект метаданных для обеспечения доступа из скриптов.
 *	Наследует QObject.
 *\_ru
 */
int AMetaObject::v_lastId = 101;

AMetaObject::AMetaObject(const QString &objectclass, 
                     const QString &objectname, 
                     AMetaObject *parent)
:QObject( parent )
{
    setObjectName( objectname );
    setClassName( objectclass );
    setDescription("");
    setId( 0 );
    if ( parent ) parent->addChild( this );
//    if ( !this->inherits("AMetaObjectGroup") ) setId( lastId() );
}


AMetaObject::~AMetaObject()
{
    AMetaObject *p = ( AMetaObject *) parent();
    if ( p && p->inherits("AMetaObject") ) p->removeChild( this );
}



Q_ULONG 
AMetaObject::id()
{
    return v_id;
}


void 
AMetaObject::setId( Q_ULONG new_id )
{
    v_id = new_id;
}


void 
AMetaObject::setClassName( const QString &name )
{
    v_className = name;
}


QString
AMetaObject::className()
{
    return v_className;
}


QString
AMetaObject::name()
{
    return objectName();
}


AMetaObject *
AMetaObject::rootObject()
{
    AMetaObject *o = parentMetaObject();
    while ( o->parentMetaObject() ) o = o->parentMetaObject();
    if ( !o->inherits("AMetaData" ) ) o = 0;
    return o;
}



AMetaObject *
AMetaObject::parentMetaObject()
{
    AMetaObject *p = (AMetaObject *) parent();
    if ( !p ) p = v_parent;
    else if ( !p->inherits("AMetaObject") ) p = 0;
    return p;
}



void 
AMetaObject::setParentMetaObject( AMetaObject * parent )
{
    v_parent = parent;
}



QString 
AMetaObject::description()
{
    return v_description;
}

void 
AMetaObject::setDescription( const QString &descr )
{
    v_description = descr;
}


void 
AMetaObject::setAttr( const QString &name, const QVariant &value )
{
    if ( value.isNull() && attrExists( name ) ) v_attr.remove( name );
    else v_attr[ name ] = value;
}



QVariant
AMetaObject::attr( const QString &name ) const
{
    return v_attr.value( name, QVariant() );
}



QVariant 
AMetaObject::attr( int idx ) const
{
    return attr( attrName( idx ) );
}



QString 
AMetaObject::attrName( int idx ) const
{
    if ( idx < 0 || idx >= attrCount() ) return QString::null;
    return v_attr.keys().at( idx );
}



bool 
AMetaObject::attrExists( const QString &name )
{
    return v_attr.contains( name );
}



int 
AMetaObject::attrCount() const
{
    return v_attr.size();
}



int 
AMetaObject::lastId()
{
    static QMutex m;

    m.lock();
    int res = v_lastId;
    v_lastId++;
    m.unlock();
    return res;
}



void 
AMetaObject::setLastId( int id )
{
    v_lastId = id;
}


int 
AMetaObject::childCount()
{
    return v_child.count();
}



AMetaObject *
AMetaObject::child( int idx )
{
    if ( idx < 0 || idx >= childCount() ) return 0;
    return v_child.values().at( idx );
}



AMetaObject *
AMetaObject::child( const QString &name )
{
    return v_child.value( name, 0 );
}



void
AMetaObject::addChild( AMetaObject * c )
{
    QString name;

    if ( c ) {
        name = c->name();
        if ( name.isEmpty() ) name = c->className();
        v_child.insertMulti( name, c );
    }
}



void
AMetaObject::removeChild( AMetaObject * c )
{
    if ( c ) v_child.remove( v_child.key( c ) );
}



/*!
 * \class AMetaCatalogue
 *
 */
AMetaCatalogue::AMetaCatalogue()
:AMetaObject("Catalogue")
{
    setId( lastId() );
    setName( QString("%1_%2").arg( tr("Catalogue") ).arg( id() ) );
}



/*!
 * \class AMetaDataInfo
 *
 */
AMetaDataInfo::AMetaDataInfo()
:AMetaObject("Info")
{
    setAppName("new");
    setAuthor("unknown");
    setDate( QDate::currentDate() );
    setLastId( 100 );

}


AMetaDataInfo::AMetaDataInfo( const AMetaDataInfo &info )
:AMetaObject("Info")
{
}


AMetaDataInfo::~AMetaDataInfo()
{
}


AMetaDataInfo& 
AMetaDataInfo::operator=(const AMetaDataInfo&)
{
    return *this;
}


QString 
AMetaDataInfo::appName()
{
    return attr("name").toString();
}


void 
AMetaDataInfo::setAppName( const QString &name )
{
    setAttr( "name", name );
}


int
AMetaDataInfo::lastId()
{
    return attr( "lastid" ).toInt();
}


void 
AMetaDataInfo::setLastId( int id )
{
    setAttr( "lastid", id );
}


QString 
AMetaDataInfo::author()
{
    return attr("author").toString();
}


void 
AMetaDataInfo::setAuthor( const QString &name )
{
    setAttr( "author", name );
}


QDate 
AMetaDataInfo::date()
{
    return attr("date").toDate();
}


void 
AMetaDataInfo::setDate( QDate d )
{
    setAttr( "date", d );
}



/*!
 * \class AMetaDataGlobal
 *
 */
AMetaDataGlobal::AMetaDataGlobal()
:AMetaObject("Global")
{
    setSourceCode("function on_systemstart()\n{\n}\n");
}


QString 
AMetaDataGlobal::sourceCode()
{
    return attr("sourcecode").toString();
}


void 
AMetaDataGlobal::setSourceCode( const QString &src )
{
    setAttr( "sourcecode", src );
}


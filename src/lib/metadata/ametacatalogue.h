/****************************************************************************
** $Id: ametacatalogue.h,v 1.1 2008/12/24 16:19:38 leader Exp $
**
** Header file of the Ananas configuration objects of Ananas
** Designer and Engine applications
**
** Created : 20031201
**
** Copyright (C) 2003-2004 Leader InfoTech.  All rights reserved.
** Copyright (C) Andrey Paskal <app at lrn dot ru>, Yoshkar-Ola
** Copyright (C) 2003-2005 Grigory Panov <gr1313 at mail dot ru >, Yoshkar-Ola
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
#ifndef AMETACATALOGUE_H
#define AMETACATALOGUE_H

#include "ametaobject.h"
#include "ametaobjectgroup.h"
#include "ametafield.h"
#include "ametaform.h"


class AMetaCatalogue: public AMetaObject
{
Q_OBJECT
public:
    AMetaCatalogue( AMetaObject *parent = 0 );

public slots:
    AMetaFields      *fields();
    AMetaFields      *gfields();
    AMetaForms       *forms();

private:
    AMetaFields      v_fields;
    AMetaFields      v_gfields;
    AMetaForms       v_forms;
};



class AMetaCatalogues: public AMetaGroup
{
Q_OBJECT
public:
    AMetaCatalogues();

    AMetaCatalogue *newCatalogue(){ return new AMetaCatalogue( this );};
    AMetaCatalogue *catalogue( const QString &name ){ return (AMetaCatalogue*) child( name );};
    AMetaCatalogue *catalogue( int idx ){ return (AMetaCatalogue*) child( idx );};
    int catalogueCount(){ return childCount(); };

public slots:


};

#endif

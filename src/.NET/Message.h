/* -*- C++ -*- */
/* ====================================================================
 * The QuickFIX Software License, Version 1.0
 *
 * Copyright (c) 2001 ThoughtWorks, Inc.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by
 *        ThoughtWorks, Inc. (http://www.thoughtworks.com/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "QuickFIX" and "ThoughtWorks, Inc." must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact quickfix-users@lists.sourceforge.net.
 *
 * 5. Products derived from this software may not be called "QuickFIX",
 *    nor may "QuickFIX" appear in their name, without prior written
 *    permission of ThoughtWorks, Inc.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THOUGHTWORKS INC OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 */

#pragma once

using namespace System;

#include "quickfix_net.h"
#include "Fields.h"
#include "Exceptions.h"
#include "Group.h"

#include "quickfix/include/Message.h"

namespace Fix
{
public __gc class Message : public IDisposable
{
public:
  Message() : disposed( false )
  {
    m_pUnmanaged = new FIX::Message();
    m_header = new Header( this );
    m_trailer = new Trailer( this );
  }

  Message( String* string ) : disposed( false )
  {
    try
    {
      if ( !String::Compare( string, String::Empty ) )
        m_pUnmanaged = new FIX::Message();
      else
        m_pUnmanaged = new FIX::Message( convertString( string ) );
      m_header = new Header( this );
      m_trailer = new Trailer( this );
    }
    catch ( FIX::InvalidMessage & e )
    { throw new InvalidMessage(); }
  }

  Message( const FIX::Message& message ) : disposed( false )
  {
    m_pUnmanaged = new FIX::Message();
    *m_pUnmanaged = message;
    m_header = new Header( this );
    m_trailer = new Trailer( this );
  }

  Message( BeginString* beginString ) : disposed( false )
  {
    m_pUnmanaged = new FIX::Message();
    m_header = new Header( this );
    m_trailer = new Trailer( this );
    setField( beginString, m_pUnmanaged->getHeader() );
  }

  Message( BeginString* beginString, MsgType* msgType ) : disposed( false )
  {
    m_pUnmanaged = new FIX::Message();
    m_header = new Header( this );
    m_trailer = new Trailer( this );
    setField( beginString, m_pUnmanaged->getHeader() );
    setField( msgType, m_pUnmanaged->getHeader() );
  }

  static bool Message::InitializeXML( String* url )
  {
    return FIX::Message::InitializeXML(convertString(url));
  }

  ~Message()
  {
    delete m_pUnmanaged;
    m_pUnmanaged = 0;
  }

  void Dispose()
  {
    if ( !disposed )
    {
      m_header->Dispose();
      m_trailer->Dispose();
      delete m_pUnmanaged; m_pUnmanaged = 0;
      System::GC::SuppressFinalize( this );
      disposed = true;
    }
  }

  void checkDisposed()
  {
    if ( disposed )
      throw new System::ObjectDisposedException( this->ToString() );
  }

  FIX::Message& unmanaged()
  { return * m_pUnmanaged; }

  void setUnmanaged( const FIX::Message& message )
  { *m_pUnmanaged = message; }

  String* ToString()
  {
    return m_pUnmanaged->getString().c_str();
  }

  String* ToXML()
  {
    return m_pUnmanaged->getXML().c_str();
  }

  void setField( StringField* field );
  void setField( BooleanField* field );
  void setField( CharField* field );
  void setField( IntField* field );
  void setField( DoubleField* field );
  void setField( UtcTimeStampField* field );
  void setField( UtcDateField* field );
  void setField( UtcTimeOnlyField* field );

  StringField* getField( StringField* field ) throw( FieldNotFound* );
  BooleanField* getField( BooleanField* field ) throw( FieldNotFound* );
  CharField* getField( CharField* field ) throw( FieldNotFound* );
  IntField* getField( IntField* field ) throw( FieldNotFound* );
  DoubleField* getField( DoubleField* field ) throw( FieldNotFound* );
  UtcTimeStampField* getField( UtcTimeStampField* field ) throw( FieldNotFound* );
  UtcDateField* getField( UtcDateField* field ) throw( FieldNotFound* );
  UtcTimeOnlyField* getField( UtcTimeOnlyField* field ) throw( FieldNotFound* );

  void addGroup( Group* group )
  {
    m_pUnmanaged->addGroup( group->unmanaged() );
  }

  Group* getGroup( unsigned num, Group* group ) throw( FieldNotFound* )
  {
    try
    {
      m_pUnmanaged->getGroup( num, group->unmanaged() );
      return group;
    }
    catch ( FIX::FieldNotFound & e )
    {
      throw new FieldNotFound( e.field );
    }
  }

__gc class Header : public IDisposable
  {
  public:
  Header( Message* message ) : m_message( message ), disposed( false ) {}
    void setField( StringField* field );
    void setField( BooleanField* field );
    void setField( CharField* field );
    void setField( IntField* field );
    void setField( DoubleField* field );
    void setField( UtcTimeStampField* field );
    void setField( UtcDateField* field );
    void setField( UtcTimeOnlyField* field );

    StringField* getField( StringField* field ) throw( FieldNotFound* );
    BooleanField* getField( BooleanField* field ) throw( FieldNotFound* );
    CharField* getField( CharField* field ) throw( FieldNotFound* );
    IntField* getField( IntField* field ) throw( FieldNotFound* );
    DoubleField* getField( DoubleField* field ) throw( FieldNotFound* );
    UtcTimeStampField* getField( UtcTimeStampField* field ) throw( FieldNotFound* );
    UtcDateField* getField( UtcDateField* field ) throw( FieldNotFound* );
    UtcTimeOnlyField* getField( UtcTimeOnlyField* field ) throw( FieldNotFound* );

    void Dispose()
    {
      if ( !disposed )
      {
        System::GC::SuppressFinalize( this );
        disposed = true;
      }
    }

    void checkDisposed()
    {
      if ( disposed )
        throw new System::ObjectDisposedException( this->ToString() );
    }

  private:
    Message* m_message;
    bool disposed;
  };

__gc class Trailer : public IDisposable
  {
  public:
    Trailer( Message* message ) : m_message( message ), disposed( false ) {}
    void setField( StringField* field );
    void setField( BooleanField* field );
    void setField( CharField* field );
    void setField( IntField* field );
    void setField( DoubleField* field );
    void setField( UtcTimeStampField* field );
    void setField( UtcDateField* field );
    void setField( UtcTimeOnlyField* field );

    StringField* getField( StringField* field ) throw( FieldNotFound* );
    BooleanField* getField( BooleanField* field ) throw( FieldNotFound* );
    CharField* getField( CharField* field ) throw( FieldNotFound* );
    IntField* getField( IntField* field ) throw( FieldNotFound* );
    DoubleField* getField( DoubleField* field ) throw( FieldNotFound* );
    UtcTimeStampField* getField( UtcTimeStampField* field ) throw( FieldNotFound* );
    UtcDateField* getField( UtcDateField* field ) throw( FieldNotFound* );
    UtcTimeOnlyField* getField( UtcTimeOnlyField* field ) throw( FieldNotFound* );

    void Dispose()
    {
      if ( !disposed )
      {
        System::GC::SuppressFinalize( this );
        disposed = true;
      }
    }

    void checkDisposed()
    {
      if ( disposed )
        throw new System::ObjectDisposedException( this->ToString() );
    }

  private:
    Message* m_message;
    bool disposed;
  };

  Header* getHeader() { checkDisposed(); return m_header; }
  Trailer* getTrailer() { checkDisposed(); return m_trailer; }

private:
  void setField( StringField* field, FIX::FieldMap& map );
  void setField( BooleanField* field, FIX::FieldMap& map );
  void setField( CharField* field, FIX::FieldMap& map );
  void setField( IntField* field, FIX::FieldMap& map );
  void setField( DoubleField* field, FIX::FieldMap& map );
  void setField( UtcTimeStampField* field, FIX::FieldMap& map );
  void setField( UtcDateField* field, FIX::FieldMap& map );
  void setField( UtcTimeOnlyField* field, FIX::FieldMap& map );

  StringField* getField( StringField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  BooleanField* getField( BooleanField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  CharField* getField( CharField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  IntField* getField( IntField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  DoubleField* getField( DoubleField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  UtcTimeStampField* getField( UtcTimeStampField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  UtcDateField* getField( UtcDateField* field, FIX::FieldMap& map ) throw( FieldNotFound* );
  UtcTimeOnlyField* getField( UtcTimeOnlyField* field, FIX::FieldMap& map ) throw( FieldNotFound* );

protected:
  Header* m_header;
  Trailer* m_trailer;
private:
  FIX::Message* m_pUnmanaged;
  bool disposed;
};
}

/*
 *
 *      File            STLVector.h
 *      Description     STL vector class
 *      Version         0.20.0801, 2011-08-01
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2011 XMSoft. All rights reserved.
 * 
 *      Contact Email: chris@xmsoft.co.kr 
 *                     xmsoft77@gmail.com	
 * 
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 */

#ifndef __STLVector_h__
#define __STLVector_h__

template <typename STLTemplate>
class STLVector
{
    public:
        typedef STLTemplate             value_type;
        typedef value_type&             reference;
        typedef const value_type&       const_reference;
        typedef value_type*             iterator;
        typedef const value_type*       const_iterator;
        typedef unsigned int            size_type;

        STLVector ( void )
        {
            m_capacity = m_size = 0;
            m_items = 0;
        }

        ~STLVector ( void )
        {
            clear ( );
        }

        STLVector& operator = ( const STLVector& v )
        {
            reserve ( v.capacity ( ) );

            iterator ptr = begin ( );
            const_iterator vbegin = v.begin ( );
            const_iterator vend = v.end ( );

            while ( vbegin != vend )
            {
                *ptr++ = *vbegin++;
            }

            m_size = v.size ( );

            return *this;
        }

        size_type size ( void ) const
        {
            return m_size;
        }

        size_type capacity ( void ) const
        {
            return m_capacity;
        }

        iterator begin ( void )
        {
            return m_items;
        }

        const_iterator begin ( void ) const
        {
            return m_items;
        }

        iterator end ( void )
        {
            return begin ( ) + size ( );
        }

        const_iterator end ( void ) const
        {
            return begin ( ) + size ( );
        }

        bool empty ( void ) const
        {
            return size ( ) == 0;
        }

        reference operator [] ( size_type pos )
        {
            return ( *( begin ( ) + pos ) );
        }

        const_reference operator [] ( size_type pos ) const
        {
            return *( begin ( ) + pos );
        }

        void clear ( void )
        {
            if ( m_capacity )
            {
                delete [] m_items;
                m_capacity = m_size = 0;
                m_items = 0;
            }
        }

        void reserve ( size_type n )
        {
            if ( capacity ( ) < n )
            {
                expand ( n );
            }
        }

        void push_back ( const value_type& x )
        {
            if ( size ( ) == capacity ( ) )
            {
                expand ( );
            }

           ( *this ) [ size ( ) ] = x;
            ++m_size;
        }

        void resize ( size_type n, value_type x )
        {
            if ( n == size ( ) )
            {
                return;
            }

            reserve ( n );
            iterator ibegin, iend;

            if ( n >= m_size )
            {
                ibegin = this->end ( );
                iend = this->begin ( ) + n;
            }
            else
            {
                ibegin = this->begin ( ) + n;
                iend = this->end ( );
            }

            while ( ibegin != iend )
            {
                *ibegin++ = x;
            }

            m_size = n;
        }

    private:

        void expand ( size_type capacity_hint = 0 )
        {
            size_type	new_capacity = ( capacity ( ) == 0 ) ? 256 : capacity ( ) * 2;

            if ( capacity_hint )
            {
                while ( new_capacity < capacity_hint )
                {
                    new_capacity *= 2;
                }
            }

            value_type*	new_items = new value_type[ new_capacity ];

            iterator ibegin = this->begin ( );
            iterator iend = this->end ( );
            value_type *ptr = new_items;

            while ( ibegin != iend )
            {
                *ptr++ = *ibegin++;
            }

            if ( m_capacity )
            {
                delete [] m_items;
            }

            m_items = new_items;
            m_capacity = new_capacity;
        }

        size_type		m_capacity;
        size_type		m_size;
        value_type*		m_items;
};

#endif 
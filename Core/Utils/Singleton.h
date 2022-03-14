/* Copyright (C) 2005-2013, Unigine Corp. All rights reserved.
 *
 * File:    Singleton.h
 * Desc:    Singleton template
 * Version: 1.05
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "Core/Types/Base.h"

/*
 */
template <class Type> class Singleton {
		
	/*
	protected:
		
		Singleton() {
			assert(object == 0 && "Singleton::Singleton(): object is not a NULL");
			object = static_cast<Type*>(this);
		}
		~Singleton() { object = 0; }
	*/
	public:
		
		static INLINE Type *get() {
			if (object == NULL)
				object = new Type();
			return object;
		}
		/*
		static INLINE Type &instance() {
			assert(object != 0 && "Singleton::instance(): object is NULL");
			return *object;
		}
		*/
	private:
		
		static Type *object;
};

/*
 */
template <class Type> Type *Singleton<Type>::object = 0;

#endif /* __SINGLETON_H__ */

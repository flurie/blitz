/***************************************************************************
 * blitz/ops.h           Function objects for math operators
 *
 * $Id$
 *
 * Copyright (C) 1997-2001 Todd Veldhuizen <tveldhui@oonumerics.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Suggestions:          blitz-dev@oonumerics.org
 * Bugs:                 blitz-bugs@oonumerics.org
 *
 * For more information, please see the Blitz++ Home Page:
 *    http://oonumerics.org/blitz/
 *
 *************************************************************************
 * $Log$
 * Revision 1.4  2002/07/02 19:11:06  jcumming
 * Rewrote and reorganized this file to make better use of macros to
 * generate all the functor classes needed to provide unary and binary
 * operators for the "new" style of expression templates.
 *
 * Revision 1.3  2002/03/06 16:06:19  patricg
 *
 * os replaced by str in the BitwiseNot template
 *
 * Revision 1.2  2001/01/24 20:22:50  tveldhui
 * Updated copyright date in headers.
 *
 * Revision 1.1.1.1  2000/06/19 12:26:12  tveldhui
 * Imported sources
 *
 * Revision 1.2  1998/03/14 00:04:47  tveldhui
 * 0.2-alpha-05
 *
 * Revision 1.1  1997/07/16 14:51:20  tveldhui
 * Update: Alpha release 0.2 (Arrays)
 *
 */

#ifndef BZ_OPS_H
#define BZ_OPS_H

#ifndef BZ_BLITZ_H
 #include <blitz/blitz.h>
#endif

#ifndef BZ_PROMOTE_H
 #include <blitz/promote.h>
#endif

#ifndef BZ_PRETTYPRINT_H
 #include <blitz/prettyprint.h>
#endif

BZ_NAMESPACE(blitz)

/*
 * Originally these function objects had no template arguments, e.g.
 *
 * struct Add {
 *     template<class T_numtype1, class T_numtype2>
 *     static inline BZ_PROMOTE(T_numtype1, T_numtype2)
 *     apply(T_numtype1 a, T_numtype2 b)
 *     { return a + b; }
 * };
 *
 * This made for neater expression templates syntax.  However, there are
 * some situations in which users may want to override type promotion
 * for certain operations.  For example, in theoretical physics, there
 * are U1 objects which when multiplied yield U1 objects, but when added
 * yield a different type.  To allow for this kind of behaviour, function
 * objects have been changed to take template parameters:
 *
 * template<class T_numtype1, class T_numtype2>
 * struct Add {
 *     typedef BZ_PROMOTE(T_numtype1, T_numtype2) T_numtype;
 *
 *     static inline T_numtype apply(T_numtype1 a, T_numtype2 b)
 *     { return a + b; }
 * };
 *
 * Type promotion is performed inside the function object.  The expression
 * templates code always looks inside the function object to determine
 * the type promotion, e.g. Add<int,float>::T_numtype
 *
 * Users are free to specialize these function objects for their own types.
 */
    
/* Unary operators that return same type as argument */
    
#define BZ_DEFINE_UNARY_OP(name,op)                         \
template<class T_numtype1>                                  \
struct name {                                               \
    typedef T_numtype1 T_numtype;                           \
                                                            \
    static inline T_numtype                                 \
    apply(T_numtype1 a)                                     \
    { return op a; }                                        \
							    \
    template<class T1>                                      \
    static inline void prettyPrint(string& str,             \
        prettyPrintFormat& format, const T1& t1)            \
    {                                                       \
        str += #op;                                         \
        t1.prettyPrint(str, format);                        \
    }                                                       \
};

BZ_DEFINE_UNARY_OP(BitwiseNot,~)
BZ_DEFINE_UNARY_OP(UnaryPlus,+)
BZ_DEFINE_UNARY_OP(UnaryMinus,-)
    
    
/* Unary operators that return a specified type */
    
#define BZ_DEFINE_UNARY_OP_RET(name,op,ret)                 \
template<class T_numtype1>                                  \
struct name {                                               \
    typedef ret T_numtype;                                  \
    static inline T_numtype                                 \
    apply(T_numtype1 a)                                     \
    { return op a; }                                        \
                                                            \
    template<class T1>                                      \
    static inline void prettyPrint(string& str,             \
        prettyPrintFormat& format, const T1& t1)            \
    {                                                       \
        str += #op;                                         \
        t1.prettyPrint(str, format);                        \
    }                                                       \
};

BZ_DEFINE_UNARY_OP_RET(LogicalNot,!,bool)
    
    
/* Binary operators that return type based on type promotion */
    
#define BZ_DEFINE_BINARY_OP(name,op)                        \
template<class T_numtype1, class T_numtype2>                \
struct name {                                               \
    typedef BZ_PROMOTE(T_numtype1, T_numtype2) T_numtype;   \
                                                            \
    static inline T_numtype                                 \
    apply(T_numtype1 a, T_numtype2 b)                       \
    { return a op b; }                                      \
							    \
    template<class T1, class T2>                            \
    static inline void prettyPrint(string& str,             \
        prettyPrintFormat& format, const T1& t1,            \
        const T2& t2)                                       \
    {                                                       \
        str += "(";                                         \
        t1.prettyPrint(str, format);                        \
        str += #op;                                         \
        t2.prettyPrint(str, format);                        \
        str += ")";                                         \
    }                                                       \
};

BZ_DEFINE_BINARY_OP(Add,+)
BZ_DEFINE_BINARY_OP(Subtract,-)
BZ_DEFINE_BINARY_OP(Multiply,*)
BZ_DEFINE_BINARY_OP(Divide,/)
BZ_DEFINE_BINARY_OP(Modulo,%)
BZ_DEFINE_BINARY_OP(BitwiseXor,^)
BZ_DEFINE_BINARY_OP(BitwiseAnd,&)
BZ_DEFINE_BINARY_OP(BitwiseOr,|)
BZ_DEFINE_BINARY_OP(ShiftRight,>>)
BZ_DEFINE_BINARY_OP(ShiftLeft,<<)
    
    
/* Binary operators that return a specified type */
    
#define BZ_DEFINE_BINARY_OP_RET(name,op,ret)                \
template<class T_numtype1, class T_numtype2>                \
struct name {                                               \
    typedef ret T_numtype;                                  \
    static inline T_numtype                                 \
    apply(T_numtype1 a, T_numtype2 b)                       \
    { return a op b; }                                      \
                                                            \
    template<class T1, class T2>                            \
    static inline void prettyPrint(string& str,             \
        prettyPrintFormat& format, const T1& t1,            \
        const T2& t2)                                       \
    {                                                       \
        str += "(";                                         \
        t1.prettyPrint(str, format);                        \
        str += #op;                                         \
        t2.prettyPrint(str, format);                        \
        str += ")";                                         \
    }                                                       \
};

BZ_DEFINE_BINARY_OP_RET(Greater,>,bool)
BZ_DEFINE_BINARY_OP_RET(Less,<,bool)
BZ_DEFINE_BINARY_OP_RET(GreaterOrEqual,>=,bool)
BZ_DEFINE_BINARY_OP_RET(LessOrEqual,<=,bool)
BZ_DEFINE_BINARY_OP_RET(Equal,==,bool)
BZ_DEFINE_BINARY_OP_RET(NotEqual,!=,bool)
BZ_DEFINE_BINARY_OP_RET(LogicalAnd,&&,bool)
BZ_DEFINE_BINARY_OP_RET(LogicalOr,||,bool)
    
    
BZ_NAMESPACE_END

#endif // BZ_OPS_H



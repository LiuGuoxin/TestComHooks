

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Feb 26 23:30:59 2016
 */
/* Compiler settings for CppCom.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __CppCom_h_h__
#define __CppCom_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFoo_FWD_DEFINED__
#define __IFoo_FWD_DEFINED__
typedef interface IFoo IFoo;

#endif 	/* __IFoo_FWD_DEFINED__ */


#ifndef __IFooLate_FWD_DEFINED__
#define __IFooLate_FWD_DEFINED__
typedef interface IFooLate IFooLate;

#endif 	/* __IFooLate_FWD_DEFINED__ */


#ifndef __FooNative_FWD_DEFINED__
#define __FooNative_FWD_DEFINED__

#ifdef __cplusplus
typedef class FooNative FooNative;
#else
typedef struct FooNative FooNative;
#endif /* __cplusplus */

#endif 	/* __FooNative_FWD_DEFINED__ */


#ifndef __FooWrapper_FWD_DEFINED__
#define __FooWrapper_FWD_DEFINED__

#ifdef __cplusplus
typedef class FooWrapper FooWrapper;
#else
typedef struct FooWrapper FooWrapper;
#endif /* __cplusplus */

#endif 	/* __FooWrapper_FWD_DEFINED__ */


#ifndef __FooLate_FWD_DEFINED__
#define __FooLate_FWD_DEFINED__

#ifdef __cplusplus
typedef class FooLate FooLate;
#else
typedef struct FooLate FooLate;
#endif /* __cplusplus */

#endif 	/* __FooLate_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CppCom_LIBRARY_DEFINED__
#define __CppCom_LIBRARY_DEFINED__

/* library CppCom */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_CppCom;

#ifndef __IFoo_INTERFACE_DEFINED__
#define __IFoo_INTERFACE_DEFINED__

/* interface IFoo */
/* [uuid][object] */ 


EXTERN_C const IID IID_IFoo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("121C468B-6EC4-48C3-8495-0909E4053398")
    IFoo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Bar( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFooVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFoo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFoo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFoo * This);
        
        HRESULT ( STDMETHODCALLTYPE *Bar )( 
            IFoo * This);
        
        END_INTERFACE
    } IFooVtbl;

    interface IFoo
    {
        CONST_VTBL struct IFooVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFoo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFoo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFoo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFoo_Bar(This)	\
    ( (This)->lpVtbl -> Bar(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFoo_INTERFACE_DEFINED__ */


#ifndef __IFooLate_INTERFACE_DEFINED__
#define __IFooLate_INTERFACE_DEFINED__

/* interface IFooLate */
/* [dual][uuid][object] */ 


EXTERN_C const IID IID_IFooLate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5327560C-D7F2-4FF3-BB2E-17EEE9875F2D")
    IFooLate : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BarLate( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IFooLateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFooLate * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFooLate * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFooLate * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFooLate * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFooLate * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFooLate * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFooLate * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BarLate )( 
            IFooLate * This);
        
        END_INTERFACE
    } IFooLateVtbl;

    interface IFooLate
    {
        CONST_VTBL struct IFooLateVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFooLate_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IFooLate_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IFooLate_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IFooLate_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IFooLate_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IFooLate_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IFooLate_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IFooLate_BarLate(This)	\
    ( (This)->lpVtbl -> BarLate(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IFooLate_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_FooNative;

#ifdef __cplusplus

class DECLSPEC_UUID("B7491852-3A2B-4CE9-8ADF-4C1D36EA2379")
FooNative;
#endif

EXTERN_C const CLSID CLSID_FooWrapper;

#ifdef __cplusplus

class DECLSPEC_UUID("876C4FD9-6F4C-4989-8408-B6443F3305E4")
FooWrapper;
#endif

EXTERN_C const CLSID CLSID_FooLate;

#ifdef __cplusplus

class DECLSPEC_UUID("0F78DDEE-D26B-4CD4-BD4A-A7DC70179D19")
FooLate;
#endif
#endif /* __CppCom_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



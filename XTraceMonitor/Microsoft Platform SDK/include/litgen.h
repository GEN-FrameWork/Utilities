#ifdef __BORLANDC__
#pragma option push -b -a8 -pc -A- /*P_O_Push*/
#endif


/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Apr 11 16:34:42 2006
 */
/* Compiler settings for .\litgen.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __litgen_h__
#define __litgen_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILITCallback_FWD_DEFINED__
#define __ILITCallback_FWD_DEFINED__
typedef interface ILITCallback ILITCallback;
#endif 	/* __ILITCallback_FWD_DEFINED__ */


#ifndef __ILITTag_FWD_DEFINED__
#define __ILITTag_FWD_DEFINED__
typedef interface ILITTag ILITTag;
#endif 	/* __ILITTag_FWD_DEFINED__ */


#ifndef __ILITHost_FWD_DEFINED__
#define __ILITHost_FWD_DEFINED__
typedef interface ILITHost ILITHost;
#endif 	/* __ILITHost_FWD_DEFINED__ */


#ifndef __ILITParserHost_FWD_DEFINED__
#define __ILITParserHost_FWD_DEFINED__
typedef interface ILITParserHost ILITParserHost;
#endif 	/* __ILITParserHost_FWD_DEFINED__ */


#ifndef __ILITCSSHost_FWD_DEFINED__
#define __ILITCSSHost_FWD_DEFINED__
typedef interface ILITCSSHost ILITCSSHost;
#endif 	/* __ILITCSSHost_FWD_DEFINED__ */


#ifndef __ILITImageHost_FWD_DEFINED__
#define __ILITImageHost_FWD_DEFINED__
typedef interface ILITImageHost ILITImageHost;
#endif 	/* __ILITImageHost_FWD_DEFINED__ */


#ifndef __ILITWriter_FWD_DEFINED__
#define __ILITWriter_FWD_DEFINED__
typedef interface ILITWriter ILITWriter;
#endif 	/* __ILITWriter_FWD_DEFINED__ */


#ifndef __ILITCallback_FWD_DEFINED__
#define __ILITCallback_FWD_DEFINED__
typedef interface ILITCallback ILITCallback;
#endif 	/* __ILITCallback_FWD_DEFINED__ */


#ifndef __ILITTag_FWD_DEFINED__
#define __ILITTag_FWD_DEFINED__
typedef interface ILITTag ILITTag;
#endif 	/* __ILITTag_FWD_DEFINED__ */


#ifndef __ILITHost_FWD_DEFINED__
#define __ILITHost_FWD_DEFINED__
typedef interface ILITHost ILITHost;
#endif 	/* __ILITHost_FWD_DEFINED__ */


#ifndef __ILITCSSHost_FWD_DEFINED__
#define __ILITCSSHost_FWD_DEFINED__
typedef interface ILITCSSHost ILITCSSHost;
#endif 	/* __ILITCSSHost_FWD_DEFINED__ */


#ifndef __ILITImageHost_FWD_DEFINED__
#define __ILITImageHost_FWD_DEFINED__
typedef interface ILITImageHost ILITImageHost;
#endif 	/* __ILITImageHost_FWD_DEFINED__ */


#ifndef __ILITWriter_FWD_DEFINED__
#define __ILITWriter_FWD_DEFINED__
typedef interface ILITWriter ILITWriter;
#endif 	/* __ILITWriter_FWD_DEFINED__ */


#ifndef __ILITParserHost_FWD_DEFINED__
#define __ILITParserHost_FWD_DEFINED__
typedef interface ILITParserHost ILITParserHost;
#endif 	/* __ILITParserHost_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ILITCallback_INTERFACE_DEFINED__
#define __ILITCallback_INTERFACE_DEFINED__

/* interface ILITCallback */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6BC62165-B91C-4993-8002-5BC30B2D1196")
    ILITCallback : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Message( 
            /* [in] */ int iType,
            /* [in] */ int iMessageCode,
            /* [string][in] */ const wchar_t *pwszMessage) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *Message )( 
            ILITCallback * This,
            /* [in] */ int iType,
            /* [in] */ int iMessageCode,
            /* [string][in] */ const wchar_t *pwszMessage);
        
        END_INTERFACE
    } ILITCallbackVtbl;

    interface ILITCallback
    {
        CONST_VTBL struct ILITCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITCallback_Message(This,iType,iMessageCode,pwszMessage)	\
    (This)->lpVtbl -> Message(This,iType,iMessageCode,pwszMessage)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITCallback_Message_Proxy( 
    ILITCallback * This,
    /* [in] */ int iType,
    /* [in] */ int iMessageCode,
    /* [string][in] */ const wchar_t *pwszMessage);


void __RPC_STUB ILITCallback_Message_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITCallback_INTERFACE_DEFINED__ */


#ifndef __ILITTag_INTERFACE_DEFINED__
#define __ILITTag_INTERFACE_DEFINED__

/* interface ILITTag */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITTag;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D655ECEB-829A-11d3-929B-00C04F68FC0F")
    ILITTag : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetName( 
            /* [size_is][in] */ const wchar_t *pwchName,
            /* [in] */ ULONG cwchName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddAttribute( 
            /* [size_is][in] */ const wchar_t *pwchName,
            /* [in] */ ULONG cwchName,
            /* [size_is][in] */ const wchar_t *pwchValue,
            /* [in] */ ULONG cwchValue) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITTagVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITTag * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITTag * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITTag * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            ILITTag * This,
            /* [size_is][in] */ const wchar_t *pwchName,
            /* [in] */ ULONG cwchName);
        
        HRESULT ( STDMETHODCALLTYPE *AddAttribute )( 
            ILITTag * This,
            /* [size_is][in] */ const wchar_t *pwchName,
            /* [in] */ ULONG cwchName,
            /* [size_is][in] */ const wchar_t *pwchValue,
            /* [in] */ ULONG cwchValue);
        
        END_INTERFACE
    } ILITTagVtbl;

    interface ILITTag
    {
        CONST_VTBL struct ILITTagVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITTag_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITTag_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITTag_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITTag_SetName(This,pwchName,cwchName)	\
    (This)->lpVtbl -> SetName(This,pwchName,cwchName)

#define ILITTag_AddAttribute(This,pwchName,cwchName,pwchValue,cwchValue)	\
    (This)->lpVtbl -> AddAttribute(This,pwchName,cwchName,pwchValue,cwchValue)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITTag_SetName_Proxy( 
    ILITTag * This,
    /* [size_is][in] */ const wchar_t *pwchName,
    /* [in] */ ULONG cwchName);


void __RPC_STUB ILITTag_SetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITTag_AddAttribute_Proxy( 
    ILITTag * This,
    /* [size_is][in] */ const wchar_t *pwchName,
    /* [in] */ ULONG cwchName,
    /* [size_is][in] */ const wchar_t *pwchValue,
    /* [in] */ ULONG cwchValue);


void __RPC_STUB ILITTag_AddAttribute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITTag_INTERFACE_DEFINED__ */


#ifndef __ILITHost_INTERFACE_DEFINED__
#define __ILITHost_INTERFACE_DEFINED__

/* interface ILITHost */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65E8FC16-8661-11d3-929C-00C04F68FC0F")
    ILITHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetId( 
            /* [retval][out] */ BSTR *pbstrId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFilename( 
            /* [retval][out] */ BSTR *pbstrFilename) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetMimeType( 
            /* [retval][out] */ BSTR *pbstrMimeType) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            ILITHost * This,
            /* [retval][out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *GetFilename )( 
            ILITHost * This,
            /* [retval][out] */ BSTR *pbstrFilename);
        
        HRESULT ( STDMETHODCALLTYPE *GetMimeType )( 
            ILITHost * This,
            /* [retval][out] */ BSTR *pbstrMimeType);
        
        END_INTERFACE
    } ILITHostVtbl;

    interface ILITHost
    {
        CONST_VTBL struct ILITHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITHost_GetId(This,pbstrId)	\
    (This)->lpVtbl -> GetId(This,pbstrId)

#define ILITHost_GetFilename(This,pbstrFilename)	\
    (This)->lpVtbl -> GetFilename(This,pbstrFilename)

#define ILITHost_GetMimeType(This,pbstrMimeType)	\
    (This)->lpVtbl -> GetMimeType(This,pbstrMimeType)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITHost_GetId_Proxy( 
    ILITHost * This,
    /* [retval][out] */ BSTR *pbstrId);


void __RPC_STUB ILITHost_GetId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITHost_GetFilename_Proxy( 
    ILITHost * This,
    /* [retval][out] */ BSTR *pbstrFilename);


void __RPC_STUB ILITHost_GetFilename_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITHost_GetMimeType_Proxy( 
    ILITHost * This,
    /* [retval][out] */ BSTR *pbstrMimeType);


void __RPC_STUB ILITHost_GetMimeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITHost_INTERFACE_DEFINED__ */


#ifndef __ILITParserHost_INTERFACE_DEFINED__
#define __ILITParserHost_INTERFACE_DEFINED__

/* interface ILITParserHost */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITParserHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("607A85B7-59C1-4b22-B873-A36334740661")
    ILITParserHost : public ILITHost
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE NewTag( 
            /* [retval][out] */ IUnknown **ppTag) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Tag( 
            /* [in] */ IUnknown *pTag,
            /* [in] */ BOOL fChildren) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Text( 
            /* [in] */ const wchar_t *pwchText,
            /* [in] */ ULONG cwchText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndChildren( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExternalStylesheet( 
            /* [in] */ IUnknown *pTag) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITParserHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITParserHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITParserHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITParserHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            ILITParserHost * This,
            /* [retval][out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *GetFilename )( 
            ILITParserHost * This,
            /* [retval][out] */ BSTR *pbstrFilename);
        
        HRESULT ( STDMETHODCALLTYPE *GetMimeType )( 
            ILITParserHost * This,
            /* [retval][out] */ BSTR *pbstrMimeType);
        
        HRESULT ( STDMETHODCALLTYPE *NewTag )( 
            ILITParserHost * This,
            /* [retval][out] */ IUnknown **ppTag);
        
        HRESULT ( STDMETHODCALLTYPE *Tag )( 
            ILITParserHost * This,
            /* [in] */ IUnknown *pTag,
            /* [in] */ BOOL fChildren);
        
        HRESULT ( STDMETHODCALLTYPE *Text )( 
            ILITParserHost * This,
            /* [in] */ const wchar_t *pwchText,
            /* [in] */ ULONG cwchText);
        
        HRESULT ( STDMETHODCALLTYPE *EndChildren )( 
            ILITParserHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *ExternalStylesheet )( 
            ILITParserHost * This,
            /* [in] */ IUnknown *pTag);
        
        HRESULT ( STDMETHODCALLTYPE *Finish )( 
            ILITParserHost * This);
        
        END_INTERFACE
    } ILITParserHostVtbl;

    interface ILITParserHost
    {
        CONST_VTBL struct ILITParserHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITParserHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITParserHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITParserHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITParserHost_GetId(This,pbstrId)	\
    (This)->lpVtbl -> GetId(This,pbstrId)

#define ILITParserHost_GetFilename(This,pbstrFilename)	\
    (This)->lpVtbl -> GetFilename(This,pbstrFilename)

#define ILITParserHost_GetMimeType(This,pbstrMimeType)	\
    (This)->lpVtbl -> GetMimeType(This,pbstrMimeType)


#define ILITParserHost_NewTag(This,ppTag)	\
    (This)->lpVtbl -> NewTag(This,ppTag)

#define ILITParserHost_Tag(This,pTag,fChildren)	\
    (This)->lpVtbl -> Tag(This,pTag,fChildren)

#define ILITParserHost_L(This,pwchText,cwchText)	\
    (This)->lpVtbl -> Text(This,pwchText,cwchText)

#define ILITParserHost_EndChildren(This)	\
    (This)->lpVtbl -> EndChildren(This)

#define ILITParserHost_ExternalStylesheet(This,pTag)	\
    (This)->lpVtbl -> ExternalStylesheet(This,pTag)

#define ILITParserHost_Finish(This)	\
    (This)->lpVtbl -> Finish(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITParserHost_NewTag_Proxy( 
    ILITParserHost * This,
    /* [retval][out] */ IUnknown **ppTag);


void __RPC_STUB ILITParserHost_NewTag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITParserHost_Tag_Proxy( 
    ILITParserHost * This,
    /* [in] */ IUnknown *pTag,
    /* [in] */ BOOL fChildren);


void __RPC_STUB ILITParserHost_Tag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITParserHost_Text_Proxy( 
    ILITParserHost * This,
    /* [in] */ const wchar_t *pwchText,
    /* [in] */ ULONG cwchText);


void __RPC_STUB ILITParserHost_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITParserHost_EndChildren_Proxy( 
    ILITParserHost * This);


void __RPC_STUB ILITParserHost_EndChildren_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITParserHost_ExternalStylesheet_Proxy( 
    ILITParserHost * This,
    /* [in] */ IUnknown *pTag);


void __RPC_STUB ILITParserHost_ExternalStylesheet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITParserHost_Finish_Proxy( 
    ILITParserHost * This);


void __RPC_STUB ILITParserHost_Finish_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITParserHost_INTERFACE_DEFINED__ */


#ifndef __ILITCSSHost_INTERFACE_DEFINED__
#define __ILITCSSHost_INTERFACE_DEFINED__

/* interface ILITCSSHost */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITCSSHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D7426056-FA2D-4d14-B36C-34D49A20D237")
    ILITCSSHost : public ILITHost
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [size_is][in] */ const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG *pcbWritten) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentLine( 
            /* [retval][out] */ ULONG *pcLine) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITCSSHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITCSSHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITCSSHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITCSSHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            ILITCSSHost * This,
            /* [retval][out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *GetFilename )( 
            ILITCSSHost * This,
            /* [retval][out] */ BSTR *pbstrFilename);
        
        HRESULT ( STDMETHODCALLTYPE *GetMimeType )( 
            ILITCSSHost * This,
            /* [retval][out] */ BSTR *pbstrMimeType);
        
        HRESULT ( STDMETHODCALLTYPE *Write )( 
            ILITCSSHost * This,
            /* [size_is][in] */ const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG *pcbWritten);
        
        HRESULT ( STDMETHODCALLTYPE *Finish )( 
            ILITCSSHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentLine )( 
            ILITCSSHost * This,
            /* [retval][out] */ ULONG *pcLine);
        
        END_INTERFACE
    } ILITCSSHostVtbl;

    interface ILITCSSHost
    {
        CONST_VTBL struct ILITCSSHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITCSSHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITCSSHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITCSSHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITCSSHost_GetId(This,pbstrId)	\
    (This)->lpVtbl -> GetId(This,pbstrId)

#define ILITCSSHost_GetFilename(This,pbstrFilename)	\
    (This)->lpVtbl -> GetFilename(This,pbstrFilename)

#define ILITCSSHost_GetMimeType(This,pbstrMimeType)	\
    (This)->lpVtbl -> GetMimeType(This,pbstrMimeType)


#define ILITCSSHost_Write(This,pb,cb,pcbWritten)	\
    (This)->lpVtbl -> Write(This,pb,cb,pcbWritten)

#define ILITCSSHost_Finish(This)	\
    (This)->lpVtbl -> Finish(This)

#define ILITCSSHost_GetCurrentLine(This,pcLine)	\
    (This)->lpVtbl -> GetCurrentLine(This,pcLine)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITCSSHost_Write_Proxy( 
    ILITCSSHost * This,
    /* [size_is][in] */ const BYTE *pb,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbWritten);


void __RPC_STUB ILITCSSHost_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITCSSHost_Finish_Proxy( 
    ILITCSSHost * This);


void __RPC_STUB ILITCSSHost_Finish_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITCSSHost_GetCurrentLine_Proxy( 
    ILITCSSHost * This,
    /* [retval][out] */ ULONG *pcLine);


void __RPC_STUB ILITCSSHost_GetCurrentLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITCSSHost_INTERFACE_DEFINED__ */


#ifndef __ILITImageHost_INTERFACE_DEFINED__
#define __ILITImageHost_INTERFACE_DEFINED__

/* interface ILITImageHost */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITImageHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D655ECEE-829A-11d3-929B-00C04F68FC0F")
    ILITImageHost : public ILITHost
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Write( 
            /* [size_is][in] */ const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG *pcbWritten) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITImageHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITImageHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITImageHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITImageHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetId )( 
            ILITImageHost * This,
            /* [retval][out] */ BSTR *pbstrId);
        
        HRESULT ( STDMETHODCALLTYPE *GetFilename )( 
            ILITImageHost * This,
            /* [retval][out] */ BSTR *pbstrFilename);
        
        HRESULT ( STDMETHODCALLTYPE *GetMimeType )( 
            ILITImageHost * This,
            /* [retval][out] */ BSTR *pbstrMimeType);
        
        HRESULT ( STDMETHODCALLTYPE *Write )( 
            ILITImageHost * This,
            /* [size_is][in] */ const BYTE *pb,
            /* [in] */ ULONG cb,
            /* [out] */ ULONG *pcbWritten);
        
        END_INTERFACE
    } ILITImageHostVtbl;

    interface ILITImageHost
    {
        CONST_VTBL struct ILITImageHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITImageHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITImageHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITImageHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITImageHost_GetId(This,pbstrId)	\
    (This)->lpVtbl -> GetId(This,pbstrId)

#define ILITImageHost_GetFilename(This,pbstrFilename)	\
    (This)->lpVtbl -> GetFilename(This,pbstrFilename)

#define ILITImageHost_GetMimeType(This,pbstrMimeType)	\
    (This)->lpVtbl -> GetMimeType(This,pbstrMimeType)


#define ILITImageHost_Write(This,pb,cb,pcbWritten)	\
    (This)->lpVtbl -> Write(This,pb,cb,pcbWritten)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITImageHost_Write_Proxy( 
    ILITImageHost * This,
    /* [size_is][in] */ const BYTE *pb,
    /* [in] */ ULONG cb,
    /* [out] */ ULONG *pcbWritten);


void __RPC_STUB ILITImageHost_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITImageHost_INTERFACE_DEFINED__ */


#ifndef __ILITWriter_INTERFACE_DEFINED__
#define __ILITWriter_INTERFACE_DEFINED__

/* interface ILITWriter */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILITWriter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9EC81687-D4F9-4b20-969A-222BC00CE50A")
    ILITWriter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetCallback( 
            /* [in] */ IUnknown *pCallback) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Create( 
            /* [string][in] */ const wchar_t *pwszLitFile,
            /* [string][in] */ const wchar_t *pwszSourceBasePath,
            /* [string][in] */ const wchar_t *pwszSource,
            /* [in] */ int iMinimumReaderVersion) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPackageHost( 
            /* [in] */ BOOL fEnforceOEB,
            /* [retval][out] */ IUnknown **ppHost) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNextCSSHost( 
            /* [retval][out] */ IUnknown **ppHost) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNextContentHost( 
            /* [retval][out] */ IUnknown **ppHost) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetNextImageHost( 
            /* [retval][out] */ IUnknown **ppHost) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Finish( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Fail( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILITWriterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILITWriter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILITWriter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILITWriter * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetCallback )( 
            ILITWriter * This,
            /* [in] */ IUnknown *pCallback);
        
        HRESULT ( STDMETHODCALLTYPE *Create )( 
            ILITWriter * This,
            /* [string][in] */ const wchar_t *pwszLitFile,
            /* [string][in] */ const wchar_t *pwszSourceBasePath,
            /* [string][in] */ const wchar_t *pwszSource,
            /* [in] */ int iMinimumReaderVersion);
        
        HRESULT ( STDMETHODCALLTYPE *GetPackageHost )( 
            ILITWriter * This,
            /* [in] */ BOOL fEnforceOEB,
            /* [retval][out] */ IUnknown **ppHost);
        
        HRESULT ( STDMETHODCALLTYPE *GetNextCSSHost )( 
            ILITWriter * This,
            /* [retval][out] */ IUnknown **ppHost);
        
        HRESULT ( STDMETHODCALLTYPE *GetNextContentHost )( 
            ILITWriter * This,
            /* [retval][out] */ IUnknown **ppHost);
        
        HRESULT ( STDMETHODCALLTYPE *GetNextImageHost )( 
            ILITWriter * This,
            /* [retval][out] */ IUnknown **ppHost);
        
        HRESULT ( STDMETHODCALLTYPE *Finish )( 
            ILITWriter * This);
        
        HRESULT ( STDMETHODCALLTYPE *Fail )( 
            ILITWriter * This);
        
        END_INTERFACE
    } ILITWriterVtbl;

    interface ILITWriter
    {
        CONST_VTBL struct ILITWriterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILITWriter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILITWriter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILITWriter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILITWriter_SetCallback(This,pCallback)	\
    (This)->lpVtbl -> SetCallback(This,pCallback)

#define ILITWriter_Create(This,pwszLitFile,pwszSourceBasePath,pwszSource,iMinimumReaderVersion)	\
    (This)->lpVtbl -> Create(This,pwszLitFile,pwszSourceBasePath,pwszSource,iMinimumReaderVersion)

#define ILITWriter_GetPackageHost(This,fEnforceOEB,ppHost)	\
    (This)->lpVtbl -> GetPackageHost(This,fEnforceOEB,ppHost)

#define ILITWriter_GetNextCSSHost(This,ppHost)	\
    (This)->lpVtbl -> GetNextCSSHost(This,ppHost)

#define ILITWriter_GetNextContentHost(This,ppHost)	\
    (This)->lpVtbl -> GetNextContentHost(This,ppHost)

#define ILITWriter_GetNextImageHost(This,ppHost)	\
    (This)->lpVtbl -> GetNextImageHost(This,ppHost)

#define ILITWriter_Finish(This)	\
    (This)->lpVtbl -> Finish(This)

#define ILITWriter_Fail(This)	\
    (This)->lpVtbl -> Fail(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ILITWriter_SetCallback_Proxy( 
    ILITWriter * This,
    /* [in] */ IUnknown *pCallback);


void __RPC_STUB ILITWriter_SetCallback_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_Create_Proxy( 
    ILITWriter * This,
    /* [string][in] */ const wchar_t *pwszLitFile,
    /* [string][in] */ const wchar_t *pwszSourceBasePath,
    /* [string][in] */ const wchar_t *pwszSource,
    /* [in] */ int iMinimumReaderVersion);


void __RPC_STUB ILITWriter_Create_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_GetPackageHost_Proxy( 
    ILITWriter * This,
    /* [in] */ BOOL fEnforceOEB,
    /* [retval][out] */ IUnknown **ppHost);


void __RPC_STUB ILITWriter_GetPackageHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_GetNextCSSHost_Proxy( 
    ILITWriter * This,
    /* [retval][out] */ IUnknown **ppHost);


void __RPC_STUB ILITWriter_GetNextCSSHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_GetNextContentHost_Proxy( 
    ILITWriter * This,
    /* [retval][out] */ IUnknown **ppHost);


void __RPC_STUB ILITWriter_GetNextContentHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_GetNextImageHost_Proxy( 
    ILITWriter * This,
    /* [retval][out] */ IUnknown **ppHost);


void __RPC_STUB ILITWriter_GetNextImageHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_Finish_Proxy( 
    ILITWriter * This);


void __RPC_STUB ILITWriter_Finish_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILITWriter_Fail_Proxy( 
    ILITWriter * This);


void __RPC_STUB ILITWriter_Fail_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILITWriter_INTERFACE_DEFINED__ */



#ifndef __LITGen_LIBRARY_DEFINED__
#define __LITGen_LIBRARY_DEFINED__

/* library LITGen */
/* [lcid][helpstring][uuid] */ 








EXTERN_C const IID LIBID_LITGen;
#endif /* __LITGen_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


#ifdef __BORLANDC__
#pragma option pop /*P_O_Pop*/
#endif

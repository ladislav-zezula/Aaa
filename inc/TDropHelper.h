/*****************************************************************************/
/* TDropHelper.h                          Copyright (c) Ladislav Zezula 2008 */
/*---------------------------------------------------------------------------*/
/* Helper classes for implementing drag&drop operations                      */
/*                                                                           */
/* To use the drag&drop support for a window (e.g. list view):               */
/*                                                                           */
/* Drop support:                                                             */
/*  - After creation of the window, create a new instance of TDropHelper     */
/*  - Query for the IID_IDropTarget and use it as parameter                  */
/*    for RegisterDragDrop call.                                             */
/*  - Don't forget to release the IDropTarget when the window is destroyed   */
/*                                                                           */
/* Drag support:                                                             */
/*  - When drag operation is being initiated (e.g. at LVN_BEGINDRAG notify), */
/*    create new instance of TDataObject.                                    */
/*  - Set the drag source window (TDropHelper::SetDragSourceWindow)          */
/*  - Set the data rendering procedure by (TDataObject::SetRenderDataProc)   */
/*  - Query the TDropHelper for IDropSource                                  */
/*  - Query the TDataObject for IDataObject                                  */
/*  - Use both interfaces as arguments for DoDragDrop                        */
/*  - Reset the drag source window (TDropHelper::SetDragSourceWindow)        */
/*  - Release IDataObject IDropSource                                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 02.04.08  1.00  Lad  The first version of TDropHelper.h                   */
/*****************************************************************************/

#ifndef __TDROPHELPER_H__
#define __TDROPHELPER_H__

typedef HDROP (*RENDER_SOURCE_DATA_PROC)(HWND hWnd);

// Represents the data object for the source of the OLE drag&drop operation
class TDataObject : public IEnumFORMATETC, public IMarshal, public IDataObject
{
    public:

    TDataObject();

    // Public functions
    void SetRenderDataProc(RENDER_SOURCE_DATA_PROC pfnRenderDataProc, HWND hWnd);

    // Functions of IUnknown interface
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // Functions of the IEnumFORMATETC
    HRESULT STDMETHODCALLTYPE Next(ULONG celt, FORMATETC * rgelt, ULONG * pceltFetched);
    HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
    HRESULT STDMETHODCALLTYPE Reset(void);
    HRESULT STDMETHODCALLTYPE Clone(IEnumFORMATETC ** ppenum);

    // Functions of the IMarshal
    HRESULT STDMETHODCALLTYPE GetUnmarshalClass(REFIID riid, void * pv, DWORD dwDestContext, void * pvDestContext, DWORD mshlflags, CLSID * pCid);
    HRESULT STDMETHODCALLTYPE GetMarshalSizeMax(REFIID riid, void * pv, DWORD dwDestContext, void * pvDestContext, DWORD mshlflags, ULONG * pSize);
    HRESULT STDMETHODCALLTYPE MarshalInterface(
  IStream * pStm,
  REFIID riid,
  void * pv,
  DWORD dwDestContext,
  void * pvDestContext,
  DWORD mshlflags
);



    // Functions of the IDataObject
    HRESULT STDMETHODCALLTYPE GetData(FORMATETC * pFormatetc, STGMEDIUM * pmedium);
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC * pFormatetc, STGMEDIUM * pmedium);
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC * pFormatetc);
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC * pFormatetcIn, FORMATETC * pFormatetcOut);
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC * pFormatetc, STGMEDIUM * pmedium, BOOL fRelease);
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC ** ppenumFormatetc);
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC * pFormatetc, DWORD advf, IAdviseSink * pAdvSink, DWORD * pdwConnection);
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD dwConnection);
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA ** ppenumAdvise);

    protected:

    ~TDataObject();

    RENDER_SOURCE_DATA_PROC m_pfnRenderDataProc;
    HWND  m_hWnd;
    ULONG m_dwFormatPos;
    ULONG m_dwRefs;
};


class TDropHelper : public IDropSource, public IDropTarget
{
    public:

    TDropHelper();

    // Functions of IUnknown interface
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // Functions of the IDropSource
    HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
    HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect);

    // Functions of the IDropTarget
    HRESULT STDMETHODCALLTYPE DragEnter(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
    HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);
    HRESULT STDMETHODCALLTYPE DragLeave(void);
    HRESULT STDMETHODCALLTYPE Drop(IDataObject * pDataObject, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect);

    protected:

    ~TDropHelper();

    IDataObject * m_pDataObject;
    HWND  m_hWnd;
    ULONG m_dwRefs;
};

#endif // __TDROPHELPER_H__

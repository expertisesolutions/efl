#ifndef __ECORE_WIN32_DND_DROP_SOURCE_H__
#define __ECORE_WIN32_DND_DROP_SOURCE_H__

#include <evil_private.h>
#include <ole2.h>

#include "Ecore_Win32.h"


class DropSource : public IDropSource
{
 private:

   LONG ref_count_;

 public: // structors

   DropSource();
   virtual ~DropSource() { }

 public: // IUnknown

   HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject);
   ULONG   __stdcall AddRef();
   ULONG   __stdcall Release();

 public: // IDropSource

   HRESULT __stdcall QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
   HRESULT __stdcall GiveFeedback(DWORD dwEffect);
};


#endif /* __ECORE_WIN32_DND_DROP_SOURCE_H__ */

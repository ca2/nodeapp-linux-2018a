#include "framework.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
//#include "xcdef.h"
//#include "xcprint.h"
//#include "xclib.h"



/*
*
*
*  xclib.c - xclip library to look after xlib mechanics for xclip
*  Copyright (C) 2001 Kim Saunders
*  Copyright (C) 2007-2008 Peter ��strand
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/*
 *
 *
 *  xcdef.h - definitions for use throughout xclip
 *  Copyright (C) 2001 Kim Saunders
 *  Copyright (C) 2007-2008 Peter Åstrand
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* output level constants */
#define OSILENT  0
#define OQUIET   1
#define OVERBOSE 2

/* generic true/false constants for stuff */
#define F 0     /* false... */
#define T 1     /* true...  */

/* true/false string constants */
#define SF "F"	/* false */
#define ST "T"	/* true  */

/*
 *
 *
 *  xclib.h - header file for functions in xclib.c
 *  Copyright (C) 2001 Kim Saunders
 *  Copyright (C) 2007-2008 Peter Åstrand
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <X11/Xlib.h>

/* xcout() contexts */
#define XCLIB_XCOUT_NONE	0	/* no context */
#define XCLIB_XCOUT_SENTCONVSEL	1	/* sent a request */
#define XCLIB_XCOUT_INCR	2	/* in an incr loop */
#define XCLIB_XCOUT_FALLBACK	3	/* UTF8_STRING failed, need fallback to XA_STRING */

/* xcin() contexts */
#define XCLIB_XCIN_NONE		0
#define XCLIB_XCIN_SELREQ	1
#define XCLIB_XCIN_INCR		2

/* functions in xclib.c */
extern int xcout(
	Display*,
	Window,
	XEvent,
	Atom,
	Atom,
	unsigned char**,
	unsigned long*,
	unsigned int*
);
extern int xcin(
	Display*,
	Window*,
	XEvent,
	Atom*,
	Atom,
	unsigned char*,
	unsigned long,
	unsigned long*,
	unsigned int*
);
extern void *xcmalloc(size_t);
extern void *xcrealloc(void*, size_t);
extern void *xcstrdup(const char *);
extern void xcmemcheck(void*);

/*
 *
 *
 *  xcprint.h - header file for functions in xcprint.c
 *  Copyright (C) 2001 Kim Saunders
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* functions in xcprint.c */
extern void prhelp(char *);
extern void prversion(void);
extern void errmalloc(void);
extern void errxdisplay(char *);
extern void errperror(int, ...);

/* check a pointer to allocater memory, print an error if it's null */
void
xcmemcheck(void *ptr)
{
   if (ptr == NULL)
errmalloc();
}

/* wrapper for malloc that checks for errors */
void *
xcmalloc(size_t size)
{
   void *mem;

   mem = malloc(size);
   xcmemcheck(mem);

   return (mem);
}

/* wrapper for realloc that checks for errors */
void *
xcrealloc(void *ptr, size_t size)
{
   void *mem;

   mem = realloc(ptr, size);
   xcmemcheck(mem);

   return (mem);
}

/* a strdup() implementation since ANSI C doesn't include strdup() */
void *
xcstrdup(const char *string)
{
   void *mem;

   /* allocate a buffer big enough to hold the characters and the
    * null terminator, then copy the string into the buffer
    */
   mem = xcmalloc(strlen(string) + sizeof(char));
   strcpy((char *) mem, string);

   return (mem);
}

/* Retrieves the contents of a selections. Arguments are:
*
* A display that has been opened.
*
* A window
*
* An event to process
*
* The selection to return
*
* The target(UTF8_STRING or XA_STRING) to return
*
* A pointer to a char array to put the selection into.
*
* A pointer to a long to record the length of the char array
*
* A pointer to an int to record the context in which to process the event
*
* Return value is 1 if the retrieval of the selection data is complete,
* otherwise it's 0.
*/
int
xcout(Display * dpy,
     Window win,
     XEvent evt, Atom sel, Atom target, unsigned char **txt, unsigned long *len,
     unsigned int *context)
{
   /* a property for other windows to put their selection into */
   static Atom pty;
   static Atom inc;
   Atom pty_type;
   Atom atomUTF8String;
   int pty_format;

   /* buffer for XGetWindowProperty to dump data into */
   unsigned char *buffer;
   unsigned long pty_size, pty_items;

   /* local buffer of text to return */
   unsigned char *ltxt = *txt;

   if (!pty) {
pty = XInternAtom(dpy, "XCLIP_OUT", False);
   }

   if (!inc) {
inc = XInternAtom(dpy, "INCR", False);
   }

   switch (*context) {
/* there is no context, do an XConvertSelection() */
   case XCLIB_XCOUT_NONE:
/* initialise return length to 0 */
if (*len > 0) {
   free(*txt);
   *len = 0;
}

/* send a selection request */
XConvertSelection(dpy, sel, target, pty, win, CurrentTime);
*context = XCLIB_XCOUT_SENTCONVSEL;
return (0);

   case XCLIB_XCOUT_SENTCONVSEL:
atomUTF8String = XInternAtom(dpy, "UTF8_STRING", False);
if (evt.type != SelectionNotify)
   return (0);

/* fallback to XA_STRING when UTF8_STRING failed */
if (target == atomUTF8String && evt.xselection.property == None) {
   *context = XCLIB_XCOUT_FALLBACK;
   return (0);
}

/* find the size and format of the data in property */
XGetWindowProperty(dpy,
  win,
  pty,
  0,
  0,
  False,
  AnyPropertyType, &pty_type, &pty_format, &pty_items, &pty_size, &buffer);
XFree(buffer);

if (pty_type == inc) {
   /* start INCR mechanism by deleting property */
   XDeleteProperty(dpy, win, pty);
   XFlush(dpy);
   *context = XCLIB_XCOUT_INCR;
   return (0);
}

/* if it's not incr, and not format == 8, then there's
* nothing in the selection (that xclip understands,
* anyway)
*/
if (pty_format != 8) {
   *context = XCLIB_XCOUT_NONE;
   return (0);
}

/* not using INCR mechanism, just read the property */
XGetWindowProperty(dpy,
  win,
  pty,
  0,
  (long) pty_size,
  False,
  AnyPropertyType, &pty_type, &pty_format, &pty_items, &pty_size, &buffer);

/* finished with property, delete it */
XDeleteProperty(dpy, win, pty);

/* copy the buffer to the pointer for returned data */
ltxt = (unsigned char *) xcmalloc(pty_items);
memcpy(ltxt, buffer, pty_items);

/* set the length of the returned data */
*len = pty_items;
*txt = ltxt;

/* free the buffer */
XFree(buffer);

*context = XCLIB_XCOUT_NONE;

/* complete contents of selection fetched, return 1 */
return (1);

   case XCLIB_XCOUT_INCR:
/* To use the INCR method, we basically delete the
* property with the selection in it, wait for an
* event indicating that the property has been created,
* then read it, delete it, etc.
*/

/* make sure that the event is relevant */
if (evt.type != PropertyNotify)
   return (0);

/* skip unless the property has a new value */
if (evt.xproperty.state != PropertyNewValue)
   return (0);

/* check size and format of the property */
XGetWindowProperty(dpy,
  win,
  pty,
  0,
  0,
  False,
  AnyPropertyType,
  &pty_type,
  &pty_format, &pty_items, &pty_size, (unsigned char **) &buffer);

if (pty_format != 8) {
   /* property does not contain text, delete it
    * to tell the other X client that we have read
    * it and to send the next property
    */
   XFree(buffer);
   XDeleteProperty(dpy, win, pty);
   return (0);
}

if (pty_size == 0) {
   /* no more data, exit from loop */
   XFree(buffer);
   XDeleteProperty(dpy, win, pty);
   *context = XCLIB_XCOUT_NONE;

   /* this means that an INCR transfer is now
    * complete, return 1
    */
   return (1);
}

XFree(buffer);

/* if we have come this far, the propery contains
* text, we know the size.
*/
XGetWindowProperty(dpy,
  win,
  pty,
  0,
  (long) pty_size,
  False,
  AnyPropertyType,
  &pty_type,
  &pty_format, &pty_items, &pty_size, (unsigned char **) &buffer);

/* allocate memory to accommodate data in *txt */
if (*len == 0) {
   *len = pty_items;
   ltxt = (unsigned char *) xcmalloc(*len);
}
else {
   *len += pty_items;
   ltxt = (unsigned char *) xcrealloc(ltxt, *len);
}

/* add data to ltxt */
memcpy(&ltxt[*len - pty_items], buffer, pty_items);

*txt = ltxt;
XFree(buffer);

/* delete property to get the next item */
XDeleteProperty(dpy, win, pty);
XFlush(dpy);
return (0);
   }

   return (0);
}

/* put data into a selection, in response to a SelecionRequest event from
* another window (and any subsequent events relating to an INCR transfer).
*
* Arguments are:
*
* A display
*
* A window
*
* The event to respond to
*
* A pointer to an Atom. This gets set to the property nominated by the other
* app in it's SelectionRequest. Things are likely to break if you change the
* value of this yourself.
*
* The target(UTF8_STRING or XA_STRING) to respond to
*
* A pointer to an array of chars to read selection data from.
*
* The length of the array of chars.
*
* In the case of an INCR transfer, the position within the array of chars
* that is being processed.
*
* The context that event is the be processed within.
*/
int
xcin(Display * dpy,
    Window * win,
    XEvent evt,
    Atom * pty, Atom target, unsigned char *txt, unsigned long len, unsigned long *pos,
    unsigned int *context)
{
   unsigned long chunk_len; /* length of current chunk (for incr
* transfers only)
*/
   XEvent res; /* response to event */
   static Atom inc;
   static Atom targets;
   static long chunk_size;

   if (!targets) {
targets = XInternAtom(dpy, "TARGETS", False);
   }

   if (!inc) {
inc = XInternAtom(dpy, "INCR", False);
   }

   /* We consider selections larger than a quarter of the maximum
      request size to be "large". See ICCCM section 2.5 */
   if (!chunk_size) {
chunk_size = XExtendedMaxRequestSize(dpy) / 4;
if (!chunk_size) {
   chunk_size = XMaxRequestSize(dpy) / 4;
}
   }

   switch (*context) {
   case XCLIB_XCIN_NONE:
if (evt.type != SelectionRequest)
   return (0);

/* set the window and property that is being used */
*win = evt.xselectionrequest.requestor;
*pty = evt.xselectionrequest.property;

/* reset position to 0 */
*pos = 0;

/* put the data into an property */
if (evt.xselectionrequest.target == targets) {
   Atom types[2] = { targets, target };

   /* send data all at once (not using INCR) */
   XChangeProperty(dpy,
   *win,
   *pty,
   XA_ATOM,
   32, PropModeReplace, (unsigned char *) types,
   (int) (sizeof(types) / sizeof(Atom))
);
}
else if (len > chunk_size) {
   /* send INCR response */
   XChangeProperty(dpy, *win, *pty, inc, 32, PropModeReplace, 0, 0);

   /* With the INCR mechanism, we need to know
    * when the requestor window changes (deletes)
    * its properties
    */
   XSelectInput(dpy, *win, PropertyChangeMask);

   *context = XCLIB_XCIN_INCR;
}
else {
   /* send data all at once (not using INCR) */
   XChangeProperty(dpy,
   *win,
   *pty, target, 8, PropModeReplace, (unsigned char *) txt, (int) len);
}

/* Perhaps FIXME: According to ICCCM section 2.5, we should
  confirm that XChangeProperty succeeded without any Alloc
  errors before replying with SelectionNotify. However, doing
  so would require an error handler which modifies a global
  variable, plus doing XSync after each XChangeProperty. */

/* set values for the response event */
res.xselection.property = *pty;
res.xselection.type = SelectionNotify;
res.xselection.display = evt.xselectionrequest.display;
res.xselection.requestor = *win;
res.xselection.selection = evt.xselectionrequest.selection;
res.xselection.target = evt.xselectionrequest.target;
res.xselection.time = evt.xselectionrequest.time;

/* send the response event */
XSendEvent(dpy, evt.xselectionrequest.requestor, 0, 0, &res);
XFlush(dpy);

/* if len < chunk_size, then the data was sent all at
* once and the transfer is now complete, return 1
*/
if (len > chunk_size)
   return (0);
else
   return (1);

break;

   case XCLIB_XCIN_INCR:
/* length of current chunk */

/* ignore non-property events */
if (evt.type != PropertyNotify)
   return (0);

/* ignore the event unless it's to report that the
* property has been deleted
*/
if (evt.xproperty.state != PropertyDelete)
   return (0);

/* set the chunk length to the maximum size */
chunk_len = chunk_size;

/* if a chunk length of maximum size would extend
* beyond the end ot txt, set the length to be the
* remaining length of txt
*/
if ((*pos + chunk_len) > len)
   chunk_len = len - *pos;

/* if the start of the chunk is beyond the end of txt,
* then we've already sent all the data, so set the
* length to be zero
*/
if (*pos > len)
   chunk_len = 0;

if (chunk_len) {
   /* put the chunk into the property */
   XChangeProperty(dpy,
   *win, *pty, target, 8, PropModeReplace, &txt[*pos], (int) chunk_len);
}
else {
   /* make an empty property to show we've
    * finished the transfer
    */
   XChangeProperty(dpy, *win, *pty, target, 8, PropModeReplace, 0, 0);
}
XFlush(dpy);

/* all data has been sent, break out of the loop */
if (!chunk_len)
   *context = XCLIB_XCIN_NONE;

*pos += chunk_size;

/* if chunk_len == 0, we just finished the transfer,
* return 1
*/
if (chunk_len > 0)
   return (0);
else
   return (1);
break;
   }
   return (0);
}


namespace lnx
{


   copydesk::copydesk(::ca::application * papp) :
      ca(papp),
      ::ca::copydesk(papp)
   {
   }

   copydesk::~copydesk()
   {
   }

   int32_t copydesk::get_file_count()
   {
//      if(!m_p->OpenClipboard())
  //       return 0;
      int32_t iCount = 0;
      throw todo(get_app());
      /* xxx HDROP hdrop = (HDROP) ::GetClipboardData(CF_HDROP);
      if(hdrop != NULL)
      {
         iCount = ::DragQueryFile(hdrop , 0xFFFFFFFF, NULL, 0);
      }
      ::CloseClipboard(); */
      return iCount;
   }


   void copydesk::get_filea(stringa & stra)
   {
      int32_t iCount = get_file_count();
      if(iCount <= 0)
         return;
//      if(!m_p->OpenClipboard())
  //       return;
      throw todo(get_app());
      /* HDROP hdrop = (HDROP) ::GetClipboardData(CF_HDROP);
      string str;
      for(int32_t i = 0; i < iCount; i++)
      {
         UINT uiLen = ::DragQueryFileW(hdrop, i, NULL, 0);
         wchar_t * lpwsz = (wchar_t *) malloc(sizeof(wchar_t) * (uiLen + 1));
         ::DragQueryFileW(hdrop, i, lpwsz, uiLen + 1);
         stra.add(::ca::international::unicode_to_utf8(lpwsz));
         free(lpwsz);
      }
      ::CloseClipboard();
      */
   }

   void copydesk::set_filea(stringa & stra)
   {

      //ASSERT(m_p->IsWindow());

      strsize iLen = 0;

      for(int32_t i = 0; i < stra.get_size(); i++)
      {
         iLen += ::ca::international::utf8_to_unicode_count(stra[i]) + 1;
      }

      throw todo(get_app());

/* xxx

      HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(DROPFILES) + (iLen + 1) * sizeof(WCHAR));
      LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock(hglbCopy);
      pDropFiles->pFiles = sizeof(DROPFILES);
      pDropFiles->pt.x = pDropFiles->pt.y = 0;
      pDropFiles->fNC = TRUE;
      pDropFiles->fWide = TRUE; // ANSI charset

      ASSERT(m_p->IsWindow());
      LPTSTR lptstrCopy = (char *) pDropFiles;
      lptstrCopy += pDropFiles->pFiles;
      wchar_t * lpwstrCopy = (wchar_t *) lptstrCopy;
      for(int32_t i = 0; i < stra.get_size(); i++)
      {
         ASSERT(m_p->IsWindow());
         ::ca::international::utf8_to_unicode(lpwstrCopy, ::ca::international::utf8_to_unicode_count(stra[i]) + 1, stra[i]);
         ASSERT(m_p->IsWindow());
         lpwstrCopy += (stra[i].get_length() + 1);
      }
      ASSERT(m_p->IsWindow());
      *lpwstrCopy = '\0';    // null character
      ASSERT(m_p->IsWindow());
      ::GlobalUnlock(hglbCopy);
      ASSERT(m_p->IsWindow());
      if(!m_p->OpenClipboard())
      {
         ::GlobalFree(hglbCopy);
         return;
      }
      EmptyClipboard();
      SetClipboardData(CF_HDROP, hglbCopy);
      VERIFY(::CloseClipboard());

*/

   }



   bool copydesk::initialize()
   {

      if(!::ca::copydesk::initialize())
         return false;




      return true;

   }


   bool copydesk::finalize()
   {

      bool bOk;

      bOk = ::ca::copydesk::finalize();

//      if(::ca::window_sp::is_set() && ::ca::window_sp::m_p->IsWindow())
      {
  //       bOk = ::ca::window_sp::m_p->DestroyWindow() != FALSE;
      }
    //  else
      {
         bOk = false;
      }

      return bOk;

   }

   void copydesk::set_plain_text(const char * psz)
   {
///      ASSERT(m_p->IsWindow());
   //   int32_t iLen = 0;

      string str;
      str = ::ca::international::utf8_to_unicode(psz);



      //ASSERT(m_p->IsWindow());
      //if(!m_p->OpenClipboard())
      {
         return;
      }

      throw todo(get_app());

/*

      EmptyClipboard();


      count iCount = ::ca::international::utf8_to_unicode_count(str) + 1;
      HGLOBAL hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, iCount * sizeof(WCHAR));
      wchar_t * lpwstrCopy  = (wchar_t *) ::GlobalLock(hglbCopy);
      ::ca::international::utf8_to_unicode(lpwstrCopy, iCount, str);
      ::GlobalUnlock(hglbCopy);

      HGLOBAL hglbCopy2 = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(CHAR) * (strlen(psz) + 1));
      char * lpstrCopy  = (char *) ::GlobalLock(hglbCopy2);
      strcpy(lpstrCopy, psz);
      ::GlobalUnlock(hglbCopy2);


      SetClipboardData(CF_UNICODETEXT, hglbCopy);
      SetClipboardData(CF_TEXT, hglbCopy2);
      VERIFY(::CloseClipboard());

*/

   }


   string copydesk::get_plain_text()
   {

      string str;
      Display * dpy;
      Window win;
      Atom target;


      /* Connect to the X server. */
      if ((dpy = XOpenDisplay(NULL)) == NULL)
      {
         return str;
      }

      target = XA_UTF8_STRING(dpy);
      Atom sseln = XA_CLIPBOARD(dpy);

      /* Create a window to trap events */
      win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 1, 1, 0, 0, 0);

      /* get events about property changes */
      XSelectInput(dpy, win, PropertyChangeMask);

      unsigned char *sel_buf; /* buffer for selection data */
      unsigned long sel_len = 0; /* length of sel_buf */
      XEvent evt; /* X Event Structures */
      unsigned int context = XCLIB_XCOUT_NONE;

      if (sseln == XA_STRING)
         sel_buf = (unsigned char *) XFetchBuffer(dpy, (int *) &sel_len, 0);
      else
      {
         while (1)
         {
            /* only get an event if xcout() is doing something */
            if (context != XCLIB_XCOUT_NONE)
               XNextEvent(dpy, &evt);

            /* fetch the selection, or part of it */
            xcout(dpy, win, evt, sseln, target, &sel_buf, &sel_len, &context);

            /* fallback is needed. set XA_STRING to target and restart the loop. */
            if (context == XCLIB_XCOUT_FALLBACK)
            {
               context = XCLIB_XCOUT_NONE;
               target = XA_STRING;
               continue;
            }

            /* only continue if xcout() is doing something */
            if (context == XCLIB_XCOUT_NONE)
               break;
         }
      }

      if(sel_len)
      {
         /* only print the buffer out, and free it, if it's not
         * empty
         */
         str = string((const char *) sel_buf, sel_len);
         if (sseln == XA_STRING)
            XFree(sel_buf);
         else
            free(sel_buf);
      }


      return str;
/* xxx
      if (IsClipboardFormatAvailable(CF_UNICODETEXT))
      {
         if(!m_p->OpenClipboard())
            return "";
         HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
         string str(::ca::international::unicode_to_utf8((const wchar_t *) GlobalLock(hglb)));
         GlobalUnlock(hglb);
         VERIFY(::CloseClipboard());
         return str;
      }
      else if (IsClipboardFormatAvailable(CF_TEXT))
      {
         if(!m_p->OpenClipboard())
            return "";
         HGLOBAL hglb = GetClipboardData(CF_TEXT);
         string str((char *) GlobalLock(hglb));
         GlobalUnlock(hglb);
         VERIFY(::CloseClipboard());
         return str;
      }
      else
      {
         return "";
      }
*/
   }

   bool copydesk::desk_to_dib(::ca::dib * pdib)
   {
//      if(!m_p->OpenClipboard())
  //       return false;
      bool bOk = false;
      throw todo(get_app());
/* xxx
      HBITMAP hbitmap = (HBITMAP) ::GetClipboardData(CF_BITMAP);
      try
      {
         ::ca::bitmap_sp bitmap(get_app());
         bitmap->Attach(hbitmap);
         //HDC hdc = ::CreateCompatibleDC(NULL);
         //::ca::graphics_sp g(get_app());
         //g->Attach(hdc);
         //::ca::graphics * pgraphics = Application.graphics_from_os_data(hdc);
         //g->SelectObject(hbitmap);
       //  BITMAP bm;
         //::GetObjectA(hbitmap, sizeof(bm), &bm);
         //if(!pdib->create(bm.bmWidth, bm.bmHeight))
           // return false;
         ::ca::graphics_sp g(get_app());
         g->SelectObject(bitmap);
         size sz = bitmap->GetBitmapDimension();
         if(pdib->create(sz))
         {
            bOk = pdib->get_graphics()->BitBlt(0, 0, sz.cx, sz.cy, g, 0, 0, SRCCOPY) != FALSE;
         }
      }
      catch(...)
      {
      }
      ::DeleteObject((HGDIOBJ) hbitmap);
      //::DeleteDC(hdc);
      ::CloseClipboard();

*/

      return bOk;
   }


} // namespace lnx




/*
 *
 *
 *  xcprint.c - functions to print help, version, errors, etc
 *  Copyright (C) 2001 Kim Saunders
 *  Copyright (C) 2007-2008 Peter Åstrand
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


/* print the help screen. argument is argv[0] from main() */
void
prhelp(char *name)
{
    fprintf(stderr,
	    "Usage: %s [OPTION] [FILE]...\n"
	    "Access an X server selection for reading or writing.\n"
	    "\n"
	    "  -i, -in          read text into X selection from standard input or files\n"
	    "                   (default)\n"
	    "  -o, -out         prints the selection to standard out (generally for\n"
	    "                   piping to a file or program)\n"
	    "  -l, -loops       number of selection requests to "
	    "wait for before exiting\n"
	    "  -d, -display     X display to connect to (eg "
	    "localhost:0\")\n"
	    "  -h, -help        usage information\n"
	    "      -selection   selection to access (\"primary\", "
	    "\"secondary\", \"clipboard\" or \"buffer-cut\")\n"
	    "      -noutf8      don't treat text as utf-8, use old unicode\n"
	    "      -version     version information\n"
	    "      -silent      errors only, run in background (default)\n"
	    "      -quiet       run in foreground, show what's happening\n"
	    "      -verbose     running commentary\n"
	    "\n" "Report bugs to <astrand@lysator.liu.se>\n", name);
//    exit(EXIT_SUCCESS);
}


/* A function to print the software version info */
void
prversion(void)
{
//    fprintf(stderr, "%s version %s\n", PACKAGE_NAME, PACKAGE_VERSION);
    fprintf(stderr, "Copyright (C) 2001-2008 Kim Saunders et al.\n");
    fprintf(stderr, "Distributed under the terms of the GNU GPL\n");
//    exit(EXIT_SUCCESS);
}

/* failure message for malloc() problems */
void
errmalloc(void)
{
    fprintf(stderr, "Error: Could not allocate memory.\n");
//    exit(EXIT_FAILURE);
}

/* failure to connect to X11 display */
void
errxdisplay(char *display)
{
    /* if the display wasn't specified, read it from the enviroment
     * just like XOpenDisplay would
     */
    if (display == NULL)
	display = getenv("DISPLAY");

    fprintf(stderr, "Error: Can't open display: %s\n", display);
//    exit(EXIT_FAILURE);
}

/* a wrapper for perror that joins multiple prefixes together. Arguments
 * are an integer, and any number of strings. The integer needs to be set to
 * the number of strings that follow.
 */
void
errperror(int prf_tot, ...)
{
    va_list ap;			/* argument pointer */
    char *msg_all;		/* all messages so far */
    char *msg_cur;		/* current message string */
    int prf_cur;		/* current prefix number */

    /* start off with an empty string */
    msg_all = (char *) xcstrdup("");

    /* start looping through the viariable arguments */
    va_start(ap, prf_tot);

    /* loop through each of the arguments */
    for (prf_cur = 0; prf_cur < prf_tot; prf_cur++) {
	/* get the current argument */
	msg_cur = va_arg(ap, char *);

	/* realloc msg_all so it's big enough for itself, the current
	 * argument, and a null terminator
	 */
	msg_all = (char *) xcrealloc(msg_all, strlen(msg_all) + strlen(msg_cur) + sizeof(char)
	    );

	/* append the current message the the total message */
	strcat(msg_all, msg_cur);
    }
    va_end(ap);

    perror(msg_all);

    /* free the complete string */
    free(msg_all);
}

/*
 * Copyright 2019 by its authors. See AUTHORS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
using System;
using System.Runtime.InteropServices;

namespace Efl.Eo
{

internal partial class NativeModule
{
   [DllImport(efl.Libs.Kernel32, CharSet = CharSet.Unicode, SetLastError = true)]
   internal static extern IntPtr LoadLibrary(string libFilename);

   [DllImport(efl.Libs.Kernel32, CharSet = CharSet.Unicode, SetLastError = true)]
   private static extern void FreeLibrary(IntPtr handle);

   ///<summary>Closes the library handle.</summary>
   ///<param name="handle">The handle to the library.</param>
   internal static void UnloadLibrary(IntPtr handle)
   {
       FreeLibrary(handle);
   }

   internal static string GetError()
   {
       return new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error()).Message;
   }

}

}

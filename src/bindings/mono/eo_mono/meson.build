mono_files += files(
  'iwrapper.cs',
  'workaround.cs',
  'FunctionWrapper.cs',
  'NativeModule.cs',
  'EoWrapper.cs',
  'WrapperSupervisor.cs'
)

if sys_windows
  mono_files += files('FunctionWrapper_Windows.cs', 'NativeModule_Windows.cs')
else
  mono_files += files('FunctionWrapper_Unix.cs', 'NativeModule_Unix.cs')
endif

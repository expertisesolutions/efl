" Vim syntax file
" Language:	EDC
" Maintainer:	billiob <billiob@gmail.com>
" Last Change:	05/08/2011

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
"
" Usage:
" 1) Copy this file to $HOME/.vim/syntax/
" 2) Paste the following line to your $HOME/.vimrc file to load the syntax
"    automatically:
"
"    au BufNewFile,BufRead *.edc set syntax=edc
"
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" A bunch of useful C keywords
syn keyword	cStructure	images data fonts collections group externals
syn keyword	cStructure	part parts dragable description rel1 rel2
syn keyword	cStatement	text image font fill origin size tag
syn keyword	cStructure	programs program script script_only lua_script lua_script_only styles style base
syn keyword	cStructure	spectra spectrum box set
syn keyword	cStructure	physics movement_freedom faces face world
syn match	cType		"+ + +;" contained

syn keyword	cLabel		item name min max type effect file
syn keyword	cLabel		mouse_events repeat_events clip_to
syn keyword	cLabel		color_class text_class x y confine
syn keyword	cLabel		state visible align step aspect aspect_preference fixed
syn keyword	cLabel		relative offset to to_x to_y normal tween
syn keyword	cLabel		border color color2 color3 font size fit align
syn keyword	cLabel		signal source action transition in target after
syn keyword	cLabel		text smooth inherit scale middle ignore_flags
syn keyword	cLabel		alias events entry_mode select_mode multiline
syn keyword	cLabel		source1 source2 source3 source4 source5 source6
syn keyword	cLabel		text_source transitions layout padding
syn keyword	cLabel		size_w size_h size_max_w size_max_h size_min_w size_min_w
syn keyword	cLabel		spread scale_hint elipsis ellipsis pointer_mode prefer
syn keyword	cLabel		precise_is_inside use_alternate_font_metrics options
syn keyword	cLabel		aspect_mode position span angle repch api
syn keyword	cLabel		external params size_range border_scale minmul
syn keyword	cLabel		insert_before insert_after

syn keyword	cLabel		mass density material restitution friction
syn keyword	cLabel		ignore_part_pos light_on damping sleep
syn keyword	cLabel		physics_body hardness linear angular
syn keyword	cLabel		backface_cull gravity rate z depth
syn keyword	cLabel		sounds sample

syn keyword	cConditional	if else switch
syn keyword	cRepeat		while for do
syn keyword	cConstant	COMP RAW LOSSY USER
syn keyword	cConstant	RECT TEXT IMAGE SWALLOW TEXTBLOCK GRADIENT SPACER
syn keyword	cConstant	GROUP BOX TABLE EXTERNAL ITEM
syn keyword	cConstant	SOLID AUTOGRAB NOGRAB
syn keyword	cConstant	NONE PLAIN OUTLINE SOFT_OUTLINE SHADOW
syn keyword	cConstant	SOFT_SHADOW OUTLINE_SHADOW OUTLINE_SOFT_SHADOW
syn keyword	cConstant	FAR_SOFT_SHADOW FAR_SHADOW GLOW
syn keyword cConstant   BOTTOM_RIGHT BOTTOM BOTTOM_LEFT LEFT
syn keyword cConstant   TOP_LEFT TOP TOP_RIGHT RIGHT
syn keyword	cConstant	STATE_SET ACTION_STOP SIGNAL_EMIT
syn keyword	cConstant	SCRIPT LUA_SCRIPT
syn keyword	cConstant	DRAG_VAL_SET DRAG_VAL_STEP DRAG_VAL_PAGE
syn keyword	cConstant	LINEAR SINUSOIDAL ACCELERATE DECELERATE SPRING
syn keyword	cConstant	VERTICAL HORIZONTAL ON_HOLD BOTH EDITABLE
syn keyword	cConstatn	DEFAULT EXPLICIT
syn keyword	cConstant	CURRENT
syn keyword	cConstant	FOCUS_SET "default" NEITHER
syn keyword	cConstant	DYNAMIC STATIC PASSWORD

syn keyword	cConstant	RIGID_BOX RIGID_SPHERE SOFT_BOX SOFT_SPHERE
syn keyword	cConstant	RIGID_CYLINDER SOFT_CYLINDER
syn keyword	cConstant	CLOTH BOUNDARY_TOP BOUNDARY_BOTTOM BOUNDARY_LEFT
syn keyword	cConstant	BOUNDARY_RIGHT BOUNDARY_FRONT BOUNDARY_BACK
syn keyword	cConstant	PHYSICS_IMPULSE PHYSICS_TORQUE_IMPULSE
syn keyword	cConstant	PHYSICS_FORCE PHYSICS_TORQUE PHYSICS_STOP
syn keyword	cConstant	PHYSICS_FORCES_CLEAR PHYSICS_VEL_SET
syn keyword	cConstant	PHYSICS_ANG_VEL_SET BOX_MIDDLE_FRONT
syn keyword	cConstant	BOX_MIDDLE_BACK BOX_FRONT BOX_BACK BOX_LEFT
syn keyword	cConstant	BOX_RIGHT BOX_TOP BOX_BOTTOM
syn keyword	cConstant	CYLINDER_MIDDLE_FRONT CYLINDER_MIDDLE_BACK
syn keyword	cConstant	CYLINDER_FRONT CYLINDER_BACK CYLINDER_CURVED
syn keyword	cConstant	CLOTH_FRONT CLOTH_BACK SPHERE_FRONT SPHERE_BACK
syn keyword	cConstant	PLAY_SAMPLE

syn keyword	cTodo		contained TODO FIXME XXX

" cCommentGroup allows adding matches for special things in comments
syn cluster	cCommentGroup	contains=cTodo

" String and Character constants
" Highlight special characters (those which have a backslash) differently
syn match	cSpecial	display contained "\\\(x\x\+\|\o\{1,3}\|.\|$\)"
if !exists("c_no_utf")
  syn match	cSpecial	display contained "\\\(u\x\{4}\|U\x\{8}\)"
endif
if exists("c_no_cformat")
  syn region	cString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial
  " cCppString: same as cString, but ends at end of line
  syn region	cCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial
else
  syn match	cFormat		display "%\(\d\+\$\)\=[-+' #0*]*\(\d*\|\*\|\*\d\+\$\)\(\.\(\d*\|\*\|\*\d\+\$\)\)\=\([hlL]\|ll\)\=\([diuoxXfeEgGcCsSpn]\|\[\^\=.[^]]*\]\)" contained
  syn match	cFormat		display "%%" contained
  syn region	cString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,cFormat
  " cCppString: same as cString, but ends at end of line
  syn region	cCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,cFormat
endif

syn match	cCharacter	"L\='[^\\]'"
syn match	cCharacter	"L'[^']*'" contains=cSpecial
if exists("c_gnu")
  syn match	cSpecialError	"L\='\\[^'\"?\\abefnrtv]'"
  syn match	cSpecialCharacter "L\='\\['\"?\\abefnrtv]'"
else
  syn match	cSpecialError	"L\='\\[^'\"?\\abfnrtv]'"
  syn match	cSpecialCharacter "L\='\\['\"?\\abfnrtv]'"
endif
syn match	cSpecialCharacter display "L\='\\\o\{1,3}'"
syn match	cSpecialCharacter display "'\\x\x\{1,2}'"
syn match	cSpecialCharacter display "L'\\x\x\+'"

"when wanted, highlight trailing white space
if exists("c_space_errors")
  if !exists("c_no_trail_space_error")
    syn match	cSpaceError	display excludenl "\s\+$"
  endif
  if !exists("c_no_tab_space_error")
    syn match	cSpaceError	display " \+\t"me=e-1
  endif
endif

"catch errors caused by wrong parenthesis and brackets
syn cluster	cParenGroup	contains=cParenError,cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserCont,cUserLabel,cBitField,cCommentSkip,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom
if exists("c_no_bracket_error")
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cCppString
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString
  syn match	cParenError	display ")"
  syn match	cErrInParen	display contained "[{}]"
else
  syn region	cParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cErrInBracket,cCppBracket,cCppString
  " cCppParen: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cErrInBracket,cParen,cBracket,cString
  syn match	cParenError	display "[\])]"
  syn match	cErrInParen	display contained "[\]{}]"
  syn region	cBracket	transparent start='\[' end=']' contains=ALLBUT,@cParenGroup,cErrInParen,cCppParen,cCppBracket,cCppString
  " cCppBracket: same as cParen but ends at end-of-line; used in cDefine
  syn region	cCppBracket	transparent start='\[' skip='\\$' excludenl end=']' end='$' contained contains=ALLBUT,@cParenGroup,cErrInParen,cParen,cBracket,cString
  syn match	cErrInBracket	display contained "[);{}]"
endif

"integer number, or floating point number without a dot and with "f".
syn case ignore
syn match	cNumbers	display transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctalError,cOctal
" Same, but without octal error (for comments)
syn match	cNumbersCom	display contained transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctal
syn match	cNumber		display contained "\d\+\(u\=l\{0,2}\|ll\=u\)\>"
"hex number
syn match	cNumber		display contained "0x\x\+\(u\=l\{0,2}\|ll\=u\)\>"
" Flag the first zero of an octal number as something special
syn match	cOctal		display contained "0\o\+\(u\=l\{0,2}\|ll\=u\)\>" contains=cOctalZero
syn match	cOctalZero	display contained "\<0"
syn match	cFloat		display contained "\d\+f"
"floating point number, with dot, optional exponent
syn match	cFloat		display contained "\d\+\.\d*\(e[-+]\=\d\+\)\=[fl]\="
"floating point number, starting with a dot, optional exponent
syn match	cFloat		display contained "\.\d\+\(e[-+]\=\d\+\)\=[fl]\=\>"
"floating point number, without dot, with exponent
syn match	cFloat		display contained "\d\+e[-+]\=\d\+[fl]\=\>"
" flag an octal number with wrong digits
syn match	cOctalError	display contained "0\o*[89]\d*"
syn case match

if exists("c_comment_strings")
  " A comment can contain cString, cCharacter and cNumber.
  " But a "*/" inside a cString in a cComment DOES end the comment!  So we
  " need to use a special type of cString: cCommentString, which also ends on
  " "*/", and sees a "*" at the start of the line as comment again.
  " Unfortunately this doesn't very well work for // type of comments :-(
  syntax match	cCommentSkip	contained "^\s*\*\($\|\s\+\)"
  syntax region cCommentString	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=cSpecial,cCommentSkip
  syntax region cComment2String	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=cSpecial
  syntax region  cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cComment2String,cCharacter,cNumbersCom,cSpaceError
  syntax region cComment	matchgroup=cCommentStart start="/\*" matchgroup=NONE end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError
else
  syn region	cCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cSpaceError
  syn region	cComment	matchgroup=cCommentStart start="/\*" matchgroup=NONE end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError
endif
" keep a // comment separately, it terminates a preproc. conditional
syntax match	cCommentError	display "\*/"
syntax match	cCommentStartError display "/\*"me=e-1 contained

syn keyword	cOperator	sizeof
if exists("c_gnu")
  syn keyword	cStatement	__asm__
  syn keyword	cOperator	typeof __real__ __imag__
endif
syn keyword	cType		int long short char void
syn keyword	cType		signed unsigned float double
if !exists("c_no_ansi") || exists("c_ansi_typedefs")
  syn keyword   cType		size_t wchar_t ptrdiff_t sig_atomic_t fpos_t
  syn keyword   cType		clock_t time_t va_list jmp_buf FILE DIR div_t ldiv_t
  syn keyword   cType		mbstate_t wctrans_t wint_t wctype_t
endif
if !exists("c_no_c99") " ISO C99
  syn keyword	cType		bool complex
  syn keyword	cType		int8_t int16_t int32_t int64_t
  syn keyword	cType		uint8_t uint16_t uint32_t uint64_t
  syn keyword	cType		int_least8_t int_least16_t int_least32_t int_least64_t
  syn keyword	cType		uint_least8_t uint_least16_t uint_least32_t uint_least64_t
  syn keyword	cType		int_fast8_t int_fast16_t int_fast32_t int_fast64_t
  syn keyword	cType		uint_fast8_t uint_fast16_t uint_fast32_t uint_fast64_t
  syn keyword	cType		intptr_t uintptr_t
  syn keyword	cType		intmax_t uintmax_t
endif
if exists("c_gnu")
  syn keyword	cType		__label__ __complex__ __volatile__
endif

syn keyword	cStructure	struct union enum typedef
syn keyword	cStorageClass	static register auto volatile extern const
if exists("c_gnu")
  syn keyword	cStorageClass	inline __attribute__
endif

if !exists("c_no_ansi") || exists("c_ansi_constants") || exists("c_gnu")
  if exists("c_gnu")
    syn keyword cConstant __GNUC__ __FUNCTION__ __PRETTY_FUNCTION__
  endif
  syn keyword cConstant __LINE__ __FILE__ __DATE__ __TIME__ __STDC__
  syn keyword cConstant __STDC_VERSION__
  syn keyword cConstant CHAR_BIT MB_LEN_MAX MB_CUR_MAX
  syn keyword cConstant UCHAR_MAX UINT_MAX ULONG_MAX USHRT_MAX
  syn keyword cConstant CHAR_MIN INT_MIN LONG_MIN SHRT_MIN
  syn keyword cConstant CHAR_MAX INT_MAX LONG_MAX SHRT_MAX
  syn keyword cConstant SCHAR_MIN SINT_MIN SLONG_MIN SSHRT_MIN
  syn keyword cConstant SCHAR_MAX SINT_MAX SLONG_MAX SSHRT_MAX
  syn keyword cConstant FLT_RADIX FLT_ROUNDS
  syn keyword cConstant FLT_DIG FLT_MANT_DIG FLT_EPSILON
  syn keyword cConstant DBL_DIG DBL_MANT_DIG DBL_EPSILON
  syn keyword cConstant LDBL_DIG LDBL_MANT_DIG LDBL_EPSILON
  syn keyword cConstant FLT_MIN FLT_MAX FLT_MIN_EXP FLT_MAX_EXP
  syn keyword cConstant FLT_MIN_10_EXP FLT_MAX_10_EXP
  syn keyword cConstant DBL_MIN DBL_MAX DBL_MIN_EXP DBL_MAX_EXP
  syn keyword cConstant DBL_MIN_10_EXP DBL_MAX_10_EXP
  syn keyword cConstant LDBL_MIN LDBL_MAX LDBL_MIN_EXP LDBL_MAX_EXP
  syn keyword cConstant LDBL_MIN_10_EXP LDBL_MAX_10_EXP
  syn keyword cConstant HUGE_VAL CLOCKS_PER_SEC NULL
  syn keyword cConstant LC_ALL LC_COLLATE LC_CTYPE LC_MONETARY
  syn keyword cConstant LC_NUMERIC LC_TIME
  syn keyword cConstant SIG_DFL SIG_ERR SIG_IGN
  syn keyword cConstant SIGABRT SIGFPE SIGILL SIGHUP SIGINT SIGSEGV SIGTERM
  " Add POSIX signals as well...
  syn keyword cConstant SIGABRT SIGALRM SIGCHLD SIGCONT SIGFPE SIGHUP
  syn keyword cConstant SIGILL SIGINT SIGKILL SIGPIPE SIGQUIT SIGSEGV
  syn keyword cConstant SIGSTOP SIGTERM SIGTRAP SIGTSTP SIGTTIN SIGTTOU
  syn keyword cConstant SIGUSR1 SIGUSR2
  syn keyword cConstant _IOFBF _IOLBF _IONBF BUFSIZ EOF
  syn keyword cConstant FOPEN_MAX FILENAME_MAX L_tmpnam
  syn keyword cConstant SEEK_CUR SEEK_END SEEK_SET
  syn keyword cConstant TMP_MAX stderr stdin stdout
  syn keyword cConstant EXIT_FAILURE EXIT_SUCCESS RAND_MAX
  " Add POSIX errors as well
  syn keyword cConstant E2BIG EACCES EAGAIN EBADF EBADMSG EBUSY
  syn keyword cConstant ECANCELED ECHILD EDEADLK EDOM EEXIST EFAULT
  syn keyword cConstant EFBIG EILSEQ EINPROGRESS EINTR EINVAL EIO EISDIR
  syn keyword cConstant EMFILE EMLINK EMSGSIZE ENAMETOOLONG ENFILE ENODEV
  syn keyword cConstant ENOENT ENOEXEC ENOLCK ENOMEM ENOSPC ENOSYS
  syn keyword cConstant ENOTDIR ENOTEMPTY ENOTSUP ENOTTY ENXIO EPERM
  syn keyword cConstant EPIPE ERANGE EROFS ESPIPE ESRCH ETIMEDOUT EXDEV
  " math.h
  syn keyword cConstant M_E M_LOG2E M_LOG10E M_LN2 M_LN10 M_PI M_PI_2 M_PI_4
  syn keyword cConstant M_1_PI M_2_PI M_2_SQRTPI M_SQRT2 M_SQRT1_2
endif
if !exists("c_no_c99") " ISO C99
  syn keyword cConstant true false
endif

syn region	cPreCondit	start="^\s*#\s*\(if\|ifdef\|ifndef\|elif\)\>" skip="\\$" end="$" end="//"me=s-1 contains=cComment,cCppString,cCharacter,cCppParen,cParenError,cNumbers,cCommentError,cSpaceError
syn match	cPreCondit	display "^\s*#\s*\(else\|endif\)\>"
if !exists("c_no_if0")
  syn region	cCppOut		start="^\s*#\s*if\s\+0\+\>" end=".\|$" contains=cCppOut2
  syn region	cCppOut2	contained start="0" end="^\s*#\s*\(endif\>\|else\>\|elif\>\)" contains=cSpaceError,cCppSkip
  syn region	cCppSkip	contained start="^\s*#\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*#\s*endif\>" contains=cSpaceError,cCppSkip
endif
syn region	cIncluded	display contained start=+"+ skip=+\\\\\|\\"+ end=+"+
syn match	cIncluded	display contained "<[^>]*>"
syn match	cInclude	display "^\s*#\s*include\>\s*["<]" contains=cIncluded
"syn match cLineSkip	"\\$"
syn cluster	cPreProcGroup	contains=cPreCondit,cIncluded,cInclude,cDefine,cErrInParen,cErrInBracket,cUserLabel,cSpecial,cOctalZero,cCppOut,cCppOut2,cCppSkip,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cString,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cParen,cBracket,cMulti
syn region	cDefine		start="^\s*#\s*\(define\|undef\)\>" skip="\\$" end="$" end="//"me=s-1 contains=ALLBUT,@cPreProcGroup
syn region	cPreProc	start="^\s*#\s*\(pragma\>\|line\>\|warning\>\|warn\>\|error\>\)" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup

syn match	cUserLabel	display "\I\i*" contained



if exists("c_minlines")
  let b:c_minlines = c_minlines
else
  if !exists("c_no_if0")
    let b:c_minlines = 50	" #if 0 constructs can be long
  else
    let b:c_minlines = 15	" mostly for () constructs
  endif
endif
exec "syn sync ccomment cComment minlines=" . b:c_minlines

" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_c_syn_inits")
  if version < 508
    let did_c_syn_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif

  HiLink cFormat		cSpecial
  HiLink cCppString		cString
  HiLink cCommentL		cComment
  HiLink cCommentStart		cComment
  HiLink cLabel			Label
  HiLink cUserLabel		Label
  HiLink cConditional		Conditional
  HiLink cRepeat		Repeat
  HiLink cCharacter		Character
  HiLink cSpecialCharacter	cSpecial
  HiLink cNumber		Number
  HiLink cOctal			Number
  HiLink cOctalZero		PreProc	 " link this to Error if you want
  HiLink cFloat			Float
  HiLink cOctalError		cError
  HiLink cParenError		cError
  HiLink cErrInParen		cError
  HiLink cErrInBracket		cError
  HiLink cCommentError		cError
  HiLink cCommentStartError	cError
  HiLink cSpaceError		cError
  HiLink cSpecialError		cError
  HiLink cOperator		Operator
  HiLink cStructure		Structure
  HiLink cStorageClass		StorageClass
  HiLink cInclude		Include
  HiLink cPreProc		PreProc
  HiLink cDefine		Macro
  HiLink cIncluded		cString
  HiLink cError			Error
  HiLink cStatement		Statement
  HiLink cPreCondit		PreCondit
  HiLink cType			Type
  HiLink cConstant		Constant
  HiLink cCommentString		cString
  HiLink cComment2String	cString
  HiLink cCommentSkip		cComment
  HiLink cString		String
  HiLink cComment		Comment
  HiLink cSpecial		SpecialChar
  HiLink cTodo			Todo
  HiLink cCppSkip		cCppOut
  HiLink cCppOut2		cCppOut
  HiLink cCppOut		Comment

  delcommand HiLink
endif

let b:current_syntax = "edc"

" vim: ts=8

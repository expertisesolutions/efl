#ifndef ELM_CODE_INDENT_H_
# define ELM_CODE_INDENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 * @brief These routines are used for calculating text indentation of lines in Elm Code.
 */

/**
 * @brief Line indent handling functions.
 * @defgroup Indent management of indentation of lines
 *
 * @{
 *
 * Functions for calculation indentation of lines within elm code.
 *
 */

ELEMENTARY_API char *elm_code_line_indent_get(Elm_Code_Line *line);

ELEMENTARY_API const char *elm_code_line_indent_matching_braces_get(Elm_Code_Line *line, unsigned int *length);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ELM_CODE_INDENT_H_ */

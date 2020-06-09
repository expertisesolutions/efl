/**
 * @defgroup efl_ui_clock EFL Ui Clock Widget
 * @ingroup Elementary
 *
 * @image html clock_inheritance_tree.png
 * @image latex clock_inheritance_tree.eps
 *
 * @image html img/widget/clock/preview-00.png
 * @image latex img/widget/clock/preview-00.eps
 *
 * @image html img/widget/clock/preview-01.png
 * @image latex img/widget/clock/preview-01.eps
 *
 * @image html img/widget/clock/preview-02.png
 * @image latex img/widget/clock/preview-02.eps
 *
 * Clock widget is used to display and input date & time values.
 * This widget displays date and time as per the <b>system's locale</b> settings (Date
 * includes Day, Month & Year along with the defined separators and
 * Time includes Hour, Minute & AM/PM fields. Separator for AM/PM field is ignored.
 *
 * The corresponding Month, AM/PM strings are displayed according to the
 * system’s language settings.
 *
 * Clock format is a combination of LIBC standard characters like
 * “%%d %%b %%Y %%I : %%M  %%p” which, as a whole represents both Date as well as Time
 * format.
 *
 * efl_ui_clock supports only the following sub set of libc date format specifiers:
 *
 * @b %%Y : The year as a decimal number including the century (example: 2011).
 *
 * @b %%y : The year as a decimal number without a century (range 00 to 99)
 *
 * @b %%m : The month as a decimal number (range 01 to 12).
 *
 * @b %%b : The abbreviated month name according to the current locale.
 *
 * @b %%B : The full month name according to the current locale.
 *
 * @b %%h : The abbreviated month name according to the current locale(same as %%b).
 *
 * @b %%d : The day of the month as a decimal number (range 01 to 31).
 *
 * @b %%e : The day of the month as a decimal number (range 1 to 31). single
 * digits are preceded by a blank.
 *
 * @b %%I : The hour as a decimal number using a 12-hour clock (range 01 to 12).
 *
 * @b %%H : The hour as a decimal number using a 24-hour clock (range 00 to 23).
 *
 * @b %%k : The hour (24-hour clock) as a decimal number (range 0 to 23). single
 * digits are preceded by a blank.
 *
 * @b %%l : The hour (12-hour clock) as a decimal number (range 1 to 12); single
 * digits are preceded by a blank.
 *
 * @b %%M : The minute as a decimal number (range 00 to 59).
 *
 * @b %%p : Either 'AM' or 'PM' according to the given time value, or the
 * corresponding strings for the current locale. Noon is treated as 'PM'
 * and midnight as 'AM'
 *
 * @b %%P : Like %p but in lower case: 'am' or 'pm' or a corresponding string for
 * the current locale.
 *
 * @b %%c : The preferred date and time representation for the current locale.
 *
 * @b %%x : The preferred date representation for the current locale without the time.
 *
 * @b %%X : The preferred time representation for the current locale without the date.
 *
 * @b %%r : The complete calendar time using the AM/PM format of the current locale.
 *
 * @b %%R : The hour and minute in decimal numbers using the format %H:%M.
 *
 * @b %%T : The time of day in decimal numbers using the format %H:%M:%S.
 *
 * @b %%D : The date using the format %%m/%%d/%%y.
 *
 * @b %%F : The date using the format %%Y-%%m-%%d.
 *
 * (For more reference on the available <b>LIBC date format specifiers</b>, please
 * visit the link:
 * http://www.gnu.org/s/hello/manual/libc.html#Formatting-Calendar-Time )
 *
 * Clock widget can provide Unicode @b separators in between its fields
 * except for AM/PM field.
 * A separator can be any <b>Unicode character</b> other than the LIBC standard
 * date format specifiers.( Example: In the format %%b %%d @b , %%y %%H @b : %%M
 * comma(,) is separator for date field %%d and colon(:) is separator for
 * hour field %%H ).
 *
 * The default format is a predefined one, based on the system Locale.
 *
 * Hour format 12hr(1-12) or 24hr(0-23) display can be selected by setting
 * the corresponding user format.
 *
 * Clock supports six fields: Year, Month, Date, Hour, Minute, AM/PM.
 *
 * The individual fields of Clock can be arranged in any order according to the format
 * set by application.
 *
 * There is a provision to set the visibility of a particular field as TRUE/ FALSE
 * so that <b>only time/ only date / only required fields</b> will be displayed.
 *
 * Each field is having a default minimum and maximum values just like the daily
 * calendar information. These min/max values can be modified as per the application usage.
 *
 * User can enter the values only in between the range of maximum and minimum.
 * Apart from these APIs, there is a provision to display only a limited set of
 * values out of the possible values. APIs to select the individual field limits
 * are intended for this purpose.
 *
 * The whole widget is left aligned and its size grows horizontally depending
 * on the current format and each field's visible/disabled state.
 *
 * This widget inherits from the @ref Elm_Layout one, so that all the
 * functions acting on it also work for Clock objects.
 *
 * This widget emits the following signals, besides the ones sent from
 * @ref Elm_Layout :
 * @li @b "changed" - whenever Clock field value is changed, this
 * signal is sent.
 * @li @b "language,changed" - whenever system locale changes, this
 * signal is sent.
 * @li @c "focused" - When the Clock has received focus. (since 1.8)
 * @li @c "unfocused" - When the Clock has lost focus. (since 1.8)
 *
 * Here is an example on its usage:
 * @li @ref clock_example
 *
 */

/**
 * @addtogroup efl_ui_clock
 * @{
 */

#include "efl_ui_clock.eo.h"

/**
 * @}
 */

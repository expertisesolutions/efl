#!/usr/bin/env python3

fixture_gen_template = """
static void
_{}_fixture(void)
{{
  _setup_window_and_widget({}, {});
}}
"""

tcase_gen_template = """
static void
{}(TCase *tc)
{{
  tcase_add_checked_fixture(tc, _{}_fixture, NULL);
  tcase_add_checked_fixture(tc, fail_on_errors_setup, fail_on_errors_teardown);
  {}(tc);
}}

"""

file_gen_template = """
#ifdef LIST_DECL
  {}
#else
  {}
#endif
"""

import sys
import json
from collections import defaultdict
from typing import Callable, Dict

output_file = sys.argv[-1]
input_files = sys.argv[1:-1]
list_of_tcases = "static const Efl_Test_Case etc[] = {\n"
list_entry = "  {{ \"{}-{}\", {}}},\n"
generated_api = ""

def interface_to_api(interface_name):
  return interface_name+"_behavior_test"

def to_func_name(class_name):
  return class_name.replace('.','_').lower()

def to_class_getter(class_name):
  return class_name.replace('.','_').upper()+'_CLASS'

tests = []

for input_file in input_files:
  with open(input_file, 'r') as content_file:
    content = content_file.read()
    start = content.index('spec-meta-start') + len('spec-meta-start')
    end = content.index('spec-meta-end')
    resulting_json = content[start:end]
    tmp = json.loads(resulting_json)
    if "test-interface" in tmp and "test-widgets" in tmp:
      tests.append(tmp)


def defaultdict_from(d: Dict, f: Callable) -> defaultdict:
  defaulted_d = defaultdict(f)
  for k, v in d.items():
    defaulted_d[k] = v
  return defaulted_d


conditional_widget_classes = defaultdict_from({
    'Efl.Ui.Vg_Animation': 'HAVE_ECTOR',
  }, str)


widgets = []
widget_hitmap = []

for test in tests:
  interface_test = to_func_name(test["test-interface"])
  if "custom-mapping" in test:
    custom_mapping = test["custom-mapping"]
  else:
    custom_mapping = {}
  for widget_class in test["test-widgets"]:
    if widget_class in custom_mapping:
      widget_test_klass = custom_mapping[widget_class]
    else:
      widget_test_klass = "WIDGET_CLASS"
    combo_name = "_{}_{}".format(to_func_name(interface_test), to_func_name(widget_class));
    combo_klass_test_name = "{}_{}".format(to_func_name(widget_class), to_func_name(widget_test_klass))

    tcase_entry = list_entry.format(interface_test, to_func_name(widget_class), combo_name)
    api = tcase_gen_template.format(combo_name, combo_klass_test_name, interface_to_api(interface_test))

    condition = conditional_widget_classes[widget_class]
    if condition:
      tcase_entry = f'#ifdef {condition}\n{tcase_entry}#endif\n'
      api = f'#ifdef {condition}\n{api}#endif\n'

    list_of_tcases += tcase_entry
    generated_api += api

    if combo_klass_test_name not in widget_hitmap:
      widgets += [{ "widget" : widget_class, "test-class" : widget_test_klass, "combo-name" : combo_klass_test_name}]
      widget_hitmap += [combo_klass_test_name]

for widget in widgets:
  fixture = fixture_gen_template.format(
    widget["combo-name"],
    to_class_getter(widget["widget"]),
    widget["test-class"]
  ) 

  condition = conditional_widget_classes[widget["widget"]]
  if condition:
    fixture = f'#ifdef {condition}\n{fixture}#endif\n'

  generated_api = fixture + generated_api

list_of_tcases += "  { NULL, NULL }\n};"

output = open(output_file, "w")
output.write(file_gen_template.format(list_of_tcases, generated_api))

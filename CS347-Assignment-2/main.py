import sys, re

out_file = open(sys.argv[2],'w')
in_file = open(sys.argv[1]).read()
# lines = in_file.splitlines()

# def comment_substituter(current_text):
# 	match = current_text.group(0)
# 	if match.startswith('"') or match.startswith('/'):
# 		return " "
# 	else:
# 		return match

def comment_remover(current_text):
	expr = re.compile(r'"(\\.|[^\\"])*"|/\*.*?\*/|//.*?$', re.DOTALL | re.MULTILINE)
	comment_substituter = " "
	modified_text = re.sub(expr, comment_substituter, current_text)
	return modified_text

# def alias_substituter(current_text):
# 	match = current_text.group(0)
# 	if match.startswith('#define'):
# 		return " "
# 	else:
# 		return match

def alias_remover(current_text):
	expr = r'(#define .*?)[\n$]'
	macros = re.findall(expr, current_text)
	alias_substituter = " "
	modified_text = re.sub(expr, alias_substituter, current_text)
	for macro in macros:
		macro_list = macro.split(' ')
		key = r'\b'+macro_list[1]+r'\b'
		replacement = ' '.join(macro_list[:2])
		modified_text = re.sub(key, replacement, modified_text)
	return modified_text

modified_text = comment_remover(in_file)
# print (modified_text)
modified_text = alias_remover(modified_text)
print (modified_text)
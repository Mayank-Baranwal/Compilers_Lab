import sys, re

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

# def alias_substituter(matching_text):
# 	match = mathching_text.group(0)
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
		# print(macro_list)
		key = r'\b'+macro_list[1]+r'\b'
		replacement = ' '.join(macro_list[2:])
		modified_text = re.sub(key, replacement, modified_text)
	return modified_text


# def getClasses:

count_inherited_class = 0
count_class = 0
count_operator_overload = 0
count_constructors = 0
count_object = 0

inherited_classes_list = list()
classes_list = list()
operator_overload_list = list()
constructors_list = list()
objects_map = {}

def getClasses(current_text):
	line=current_text

	global count_inherited_class
	global count_class
	global count_constructors
	global count_operator_overload

	global inherited_classes_list
	global classes_list
	global operator_overload_list
	global constructors_list

	class_regex = r'\bclass\b\s+([A-Za-z_]\w*)\s*\{'
	classes = re.findall(class_regex, line)

	class_regex2 = r'(\bclass\b\s+[A-Za-z_]\w*)\s*\{'
	classes2 = re.findall(class_regex2, line)

	inherited_class_regex=r'\bclass\b\s+([A-Za-z_]\w*)\s*\:\s*(?:((?:private|protected|public)?\s+(?:[A-Za-z_]\w*))\s*\,?\s*)+\{'
	inherited_classes=re.findall(inherited_class_regex, line)

	inherited_class_regex2=r'(\bclass\b\s+[A-Za-z_]\w*\s*\:\s*(?:(?:private|protected|public)?\s+(?:[A-Za-z_]\w*)\s*\,?\s*)*(?:private|protected|public)?\s+(?:[A-Za-z_]\w*))\s*\{'
	inherited_classes2=re.findall(inherited_class_regex2, line)

	for class_item in classes:
		classes_list.append(class_item)

	for class_item in classes2:
		new_line_num = class_item.count("\n")
		count_class += 1 + new_line_num

	for inherited_class in inherited_classes:
		inherited_classes_list.append(inherited_class[0])
		classes_list.append(inherited_class[0])

	for inherited_class in inherited_classes2:
		new_line_num = str(inherited_class).count("\n")
		# print(new_line_num)
		# print(inherited_class)
		count_class += 1 + new_line_num
		count_inherited_class += 1 + new_line_num
		# print(inherited_class)


def getOverloadedFunctions(current_text):
	line=current_text
	# print(line)
	global count_operator_overload
	global operator_overload_list

	operators_regex=r'(\+=|-=|\*=|/=|%=|\^=|&=|\|=|==|!=|<=|>=|<=>|<<|>>|>>=|<<=|&&|\|\||\+\+|--|\,|->\*|\\->|\+|-|\*|/|%|\^|&|\||~|!|=|<|>|\(\s*\)|\[\s*\])'
	operators_overload_regex=r'\boperator\b\s*' + operators_regex + r'\s*([^\)]*\))[^\{\;]*?\{'
	operators_overload=re.findall(operators_overload_regex, line);

	operators_regex2=r'(\+=|-=|\*=|/=|%=|\^=|&=|\|=|==|!=|<=|>=|<=>|<<|>>|>>=|<<=|&&|\|\||\+\+|--|\,|->\*|\\->|\+|-|\*|/|%|\^|&|\||~|!|=|<|>|\(\s*\)|\[\s*\])'
	operators_overload_regex2=r'(\s*\boperator\b\s*' + operators_regex2 + r'\s*(?:[^\)]*\)))[^\{\;]*?\{'
	operators_overload2=re.findall(operators_overload_regex2, line);

	for operators_overload_item in operators_overload:
		operator_overload_list.append(operators_overload_item[0]+operators_overload_item[1])

	for operators_overload_item in operators_overload2:
		temp=operators_overload_item[0]+operators_overload_item[1]
		new_line_count=temp.count("\n")
		count_operator_overload += 1 + new_line_count

def getConstructors (current_text) :
	global count_inherited_class
	global count_class
	global count_constructors
	global count_operator_overload

	global inherited_classes_list
	global classes_list
	global operator_overload_list
	global constructors_list

	line = current_text
	constructor_reg_exp = r'(?:[^~]|^)\b([A-Za-z_][\w\s]*?(?:\:\:)?[\w\s]*)\s*\(([^)]*?)\)?\s*[\{\:]'
	constructor_reg_exp2 = r'(?:[^~]|^)\b([A-Za-z_][\w\s]*?(?:\:\:)?[\w\s]*\s*\([^)]*?\)?)\s*[\{\:]'

	list_constructors = re.findall(constructor_reg_exp, line)
	list_constructors2 = re.findall(constructor_reg_exp2, line)
	print(len(list_constructors))
	print(len(list_constructors2))

	i=0
	for constructor in list_constructors:
		constructor2=list_constructors2[i]
		# print(constructor2)
		
		i += 1
		group_name = constructor[0]
		names = group_name.split('::')
		new_list = list()
		for name in names:
			new_list.append(name.strip())
		names = new_list
		constructor_name = names[-1]
		# print(constructor_name)
		if constructor_name in classes_list:
			if len(names) == 1 or names[-1] == names[-2]:
				constructor = [x.replace('\n', '').replace(' ', '').replace('\t', '') for x in constructor]
				constructors_list.append(constructor)
				new_line_count = constructor2.count('\n')
				print(constructor2)
				count_constructors += 1 + new_line_count

def getObjects (current_text):
	line=current_text
	global count_object
	global objects_map
	global classes_list

	object_regex=r'([A-Za-z_]\w*)\b\s*([\s\*]*[A-Za-z_\,][A-Za-z0-9_\.\,\[\]\s\(\)]*[^\n<>]*?);'
	objects=re.findall(object_regex, line);

	object_regex2=r'([A-Za-z_]\w*\b\s*[\s\*]*[A-Za-z_\,][A-Za-z0-9_\.\,\[\]\s\(\)]*[^\n<>]*?);'
	objects2=re.findall(object_regex2, line);

	i=0
	for object_item in objects:
		object_item2=objects2[i]
		i+=1
		if object_item[0] in classes_list:
			if object_item[0] not in objects_map:
				objects_map[object_item[0]] = ''
			objects_map[object_item[0]] += (object_item[1].replace('\n','').replace('\t','')+', ')
			new_line_count = object_item2.count("\n")
			count_object += 1 + new_line_count
		


def print_to_file(file):
	global count_inherited_class
	global count_class
	global count_constructors
	global count_operator_overload
	global count_object

	global inherited_classes_list
	global classes_list
	global operator_overload_list
	global constructors_list
	global objects_map

	with open(file, "a") as f:
		f.write("C++ Code Analyser Output: \n")
		f.write("Number of lines of Object Declarations:\t\t\t\t" + str(count_object) + '\n')
		f.write("Number of lines of Class Definitions:\t\t\t\t" + str(count_class) + '\n')
		f.write("Number of lines of Constructor Definitions:\t\t\t" + str(count_constructors) + '\n')
		f.write("Number of lines of Inherited Class Definitions:\t\t\t" + str(count_inherited_class) + '\n')
		f.write("Number of lines of Operator Overloaded Function Definitions:\t" + str(count_operator_overload) + '\n')
		f.write("\n")
		f.write("Object Declarations:\n")

		for object in objects_map:
			f.write(object + ": " + objects_map[object] + "\n")
		f.write("\nClass Definitions: " + str(classes_list) + "\n")
		f.write("\nConstructor Definitions: " + str(constructors_list) + "\n")
		f.write("\nInherited Class Definitions: " + str(inherited_classes_list) + "\n")
		f.write("\nOperator Overloaded Function Definitions: " + str(operator_overload_list) + "\n")


if __name__ == '__main__':
	out_file = open("output.txt",'w')
	in_file = open("input.txt").read()
	in_file = comment_remover(in_file)
	in_file = alias_remover(in_file)
	# print (in_file)
	getClasses(in_file)
	getOverloadedFunctions(in_file)
	getConstructors(in_file)
	getObjects(in_file)
	print_to_file("output.txt")

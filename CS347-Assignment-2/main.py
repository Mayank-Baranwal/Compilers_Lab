import sys, re

# lines = in_file.splitlines()


# def getClasses:

count_inherited_class = 0
count_class = 0
count_operator_overload = 0
count_constructors = 0

inherited_classes_list=list()
classes_list=list()
operator_overload_list=list()

def getClasses(current_text):
	individual_lines=current_text.split('\n')
	global count_inherited_class
	global count_class
	global inherited_classes_list
	global classes_list

	for line in individual_lines:
		line = line + '\n'
		class_regex = r'\bclass\b\s+([A-Za-z_]\w*)\s*[\n\{]'
		classes = re.findall(class_regex, line)

		inherited_class_regex=r'\bclass\b\s+([A-Za-z_]\w*)\s*\:\s*((?:private|protected|public)?\s+(?:[A-Za-z_]\w*)\s*\,?\s*)+[\n\{]'
		inherited_classes=re.findall(inherited_class_regex, line)

		is_present_class = False
		is_present_inherited_class = False

		if len(classes)>0:
			is_present_class=True
		if len(inherited_classes)>0:
			is_present_inherited_class=True

		if is_present_class:
			count_class += len(inherited_classes)
		if is_present_inherited_class:
			count_inherited_class +=len(inherited_classes)
			count_class += len(inherited_classes)

		for class_item in classes:
			classes_list.append(class_item)
			# print(class_item)
		for inherited_class in inherited_classes:
			inherited_classes_list.append(inherited_class)
			classes_list.append(inherited_class[0])
			# print(inherited_class[0])


def getOverloadedFunctions(current_text):
	individual_lines=current_text.split('\n')
	global count_operator_overload
	global operator_overload_list

	for line in individual_lines:
		line = line + '\n'
		operators_regex=r'(\+=|-=|\*=|/=|%=|\^=|&=|\|=|==|!=|<=|>=|<=>|<<|>>|>>=|<<=|&&|\|\||\+\+|--|\,|->\*|\\->|\+|-|\*|/|%|\^|&|\||~|!|=|<|>|\(\s*\)|\[\s*\])'
		operators_overload_regex=r'\boperator\b\s*' + operators_regex + r'\s*([^\{\;]*)?[\n\{]'

		operators_overload=re.findall(operators_overload_regex, line);

		is_present_operator_overload = False

		if len(operators_overload)>0:
			is_present_operator_overload=True
		if is_present_operator_overload:
				count_operator_overload += len(operators_overload)
		for operators_overload_item in operators_overload:
				classes_list.append(operators_overload_item)
				print(operators_overload_item)

def getConstructors (current_text) :
    lines = current_text.split('\n')
    constructor_reg_exp = r'(?:[^~]|^)\b([A-Za-z_][\w\:\s]*)\s*\(([^)]*?)\)?\s*[\n\{\:]'
    for line in lines:
        is_constructor = False
        line = line + '\n'
        list_constructors = re.findall(constructor_reg_exp, line)
        for constructor in list_constructors:
            group_name = constructor[0]
            names = group_name.split('::')
            for name in names:
                name = name.strip()
            constructor_name = names[-1]
            if constructor_name in classes_list:
                if len(names) == 1 or names[-1] == names[-2]:
                    is_constructor = True
                    constructors_names_list.append(constructor)
        if is_constructor:
            count_constructors = count_constructors + 1

# out_file = open(sys.argv[2],'w')
# in_file = open(sys.argv[1]).read()
# getClasses(in_file)
# getOverloadedFunctions(in_file)



if __name__ == '__main__':
    # out_file = open(sys.argv[2],'w')
    in_file = open(sys.argv[1]).read()
    getClasses(in_file)
    getOverloadedFunctions(in_file)
    getConstructors(in_file)

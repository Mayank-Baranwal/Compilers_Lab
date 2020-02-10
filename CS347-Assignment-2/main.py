import sys, re

def counting_constructors (current_text) :
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

if __name__ == '__main__':
    # out_file = open(sys.argv[2],'w')
    in_file = open(sys.argv[1]).read()
    count_constructors(in_file)

import re
import argparse
import xml.etree.ElementTree as ET
import sys

def getType(var):
    if var is True or var is False:
        return "bool"
    elif var is None:
        return "nil"
    elif isinstance(var, int):
        return "int"
    elif isinstance(var, str):
        return "string"
    else:
        return ""

def toInt(var):
    try:
        return int(var)
    except:
        exit(32)

def toBool(var):
    if var.lower() == "true":
        return True
    elif var.lower() == "false":
        return False
    else:
        exit(32)

def toStr(var):
    try:
        return str(var)
    except:
        exit(32)

class Frame:
    def __init__(self, parent=None):
        self.parent = parent
        self.undefined_variables = {}
        self.variables = {}

    def get_variable(self, name):
        if name in self.undefined_variables:
            exit(56)
        elif name in self.variables:
            return self.variables[name]
        elif self.parent is not None:
            return self.parent.get_variable(name)
        else:
            exit(54)

    def set_variable(self, name, value):
        if name not in self.variables and name not in self.undefined_variables:
            if self.parent is None:
                exit(54)
            else:
                self.parent.set_variable(name ,value)
        else:
            if name in self.undefined_variables:
                self.undefined_variables.pop(name)
            self.variables[name] = value
    
    def create_variable(self, name, value):
        if name in self.variables or name in self.undefined_variables:
            exit(52)
        else:
            self.undefined_variables[name] = value

    def print_frame(self, frame):
        print(f"{frame} variables:")
        for variable in self.variables:
            print(variable)

class Stack:
    def __init__(self):
        self.temp_frame = None
        self.frames = [Frame(parent=None)] # Global frame only after intialization

    def create_frame(self):
        self.temp_frame = Frame(parent=None)

    def push_frame(self):
        if self.temp_frame is None:
            exit(55)
        if self.current_frame() is None:
            self.temp_frame.parent = self.frames[0]
        else:
            self.temp_frame.parent = self.current_frame()
        self.frames.append(self.temp_frame)
        self.temp_frame = None

    def pop_frame(self):
        if len(self.frames) == 1:
            exit(55)
        else:
            self.temp_frame = self.frames.pop()

    def current_frame(self):
        if len(self.frames) == 1:
            return None
        else:
            return self.frames[-1]

    def get_variable(self, name):
        var_frame = name[:2]
        var_name = name[3:]
        if var_frame == "GF":
            return self.frames[0].get_variable(var_name)
        elif var_frame == "LF":
            frame = self.current_frame()
            if frame is None:
                exit(55)
            return frame.get_variable(var_name)
        elif var_frame == "TF":
            if self.temp_frame is not None:
                return self.temp_frame.get_variable(var_name)
            else:
                exit(55)

    def set_variable(self, name, value):
        var_frame = name[:2]
        var_name = name[3:]
        if var_frame == "GF":
            self.frames[0].set_variable(var_name, value)
        elif var_frame == "LF":
            frame = self.current_frame()
            if frame is None:
                exit(55)
            frame.set_variable(var_name, value)
        elif var_frame == "TF":
            if self.temp_frame is not None:
                self.temp_frame.set_variable(var_name, value)
            else:
                exit(55)

    def create_variable(self, name, value):
        var_frame = name[:2]
        var_name = name[3:]
        if var_frame == "GF":
            self.frames[0].create_variable(var_name, value)
        elif var_frame == "LF":
            frame = self.current_frame()
            if frame is None:
                exit(55)
            frame.create_variable(var_name, value)
        elif var_frame == "TF":
            if self.temp_frame is not None:
                self.temp_frame.create_variable(var_name, value)
            else:
                exit(55)
    
    def print_frames(self):
        if self.temp_frame is not None:
            self.temp_frame.print_frame("TF")
        for frame in self.frames:
            if frame == self.frames[0]:
                frame.print_frame("GF")
            else:
                frame.print_frame("LF")

class Arg:
    def __init__(self, type, value):
        self.type = type
        self.value = value

class Instruction:
    def __init__(self, name):
        self.name = name
        self.args = []
    
    def add_argument(self, type, value):
        self.args.append(Arg(type, value))

    def get_argument(self, index):
        if -1 < index and index < len(self.args):
            return self.args[index]


parser = argparse.ArgumentParser(description='Description of the script')
parser.add_argument('--source', help='An input file with an XML representation of the source code')
parser.add_argument('--input', help='A file with inputs for the interpretation of the specified source code itself')

args = vars(parser.parse_args())

if args['source'] is None and args['input'] is None:
    print('At least one of the parameters (--source or --input) must always be specified.')
    exit(10)

if args['source'] is not None:
    try:
        source = open(args['source'], 'r')
    except (FileNotFoundError, PermissionError) as e:
        print('Error:', e)
        exit(11)
else:
    source = sys.stdin

if args['input'] is not None:
    try:
        input = open(args['input'], 'r')
    except (FileNotFoundError, PermissionError) as e:
        print('Error:', e)
        exit(11)
else:
    input = sys.stdin

#Load XML
try:
    tree = ET.parse(source)
except:
    exit(31)
root = tree.getroot()
try:
    root[:] = sorted(root, key=lambda instruction: int(instruction.get('order'))) # Sort instructions based on order
except:
    exit(32)

#Check XML
if root.tag != 'program':
   exit(32)

if root.get('language') != "IPPcode23":
    exit(32)

last_order = -1
for instruction_index, instruction in enumerate(root):
    instruction[:] = sorted(instruction, key=lambda arg: arg.tag) # Sort arguments based on tag

    if instruction.tag != 'instruction':
        exit(32)

    if instruction.get('opcode') is None:
        exit(32)

    if instruction.get('order') is None:
        exit(32)

    if int(instruction.get('order')) <= 0:
        exit(32)
    if  last_order >= int(instruction.get('order')):
        exit(32)
    last_order = int(instruction.get('order'))

    for arg_index, arg in enumerate(instruction):
        if not re.match(r"^arg" + re.escape(str(arg_index + 1)) + "$", arg.tag):
            exit(32)
        if arg.get('type') is None:
            exit(32)

    arg_count = len(instruction)
    match instruction.get('opcode').upper():
        #<INSTRUCTION>
        case 'CREATEFRAME' | 'PUSHFRAME' | 'POPFRAME' | 'RETURN' | 'BREAK':
            if arg_count != 0:
                exit(32)
        
        #<INSTRUCTION> <label>
        case 'CALL' | 'LABEL' | 'JUMP':
            if arg_count != 1:
                exit(32)
        
        #<INSTRUCTION> <var>
        case 'DEFVAR' | 'CALL' | 'POPS':
            if arg_count != 1:
                exit(32)
        
        #<INSTRUCTION> <symb>
        case 'PUSHS' | 'WRITE' | 'EXIT' | 'DPRINT':
            if arg_count != 1:
                print("Idem")
                exit(32)
        
        #<INSTRUCTION> <var> <symb>
        case 'MOVE' | 'INT2CHAR' | 'STRLEN' | 'TYPE' | 'NOT':
            if arg_count != 2:
                exit(32)
        
        #<INSTRUCTION> <var> <type>
        case 'READ':
            if arg_count != 2:
                exit(32)
        
        #<INSTRUCTION> <var> <symb1> <symb2>
        case 'ADD' | 'SUB' | 'MUL' | 'IDIV' | 'LT' | 'GT' | 'EQ' | 'AND' | 'OR' | 'STRI2INT' | 'CONCAT' | 'GETCHAR' | 'SETCHAR':
            if arg_count != 3:
                exit(32)
        
        #<INSTRUCTION> <label> <symb1> <symb2>
        case 'JUMPIFEQ' | 'JUMPIFNEQ':
            if arg_count != 3:
                exit(32)
        
        case _:
            exit(32)


instructions = []
for index, instruction in enumerate(root):
    to_append = Instruction(instruction.get('opcode'))
    
    for arg in instruction:
        to_append.add_argument(arg.get('type'), arg.text)
    
    instructions.append(to_append)

labels = {}
for pc, instruction in enumerate(instructions):
    if instruction.name == "LABEL":
        label = instruction.get_argument(0)
        if label.value in labels:
            exit(52)
        else:
            labels[label.value] = pc + 1

data_stack = []
frame_stack = Stack()
call_stack = []
pc = 0
instruction_count = 0
while True:
    if pc >= len(instructions):
        break
    instruction = instructions[pc]
    instruction_count += 1
    match instruction.name.upper():
        #<INSTRUCTION>
        case 'CREATEFRAME':
            frame_stack.create_frame()

        case 'PUSHFRAME':
            frame_stack.push_frame()

        case 'POPFRAME':
            frame_stack.pop_frame()

        case 'RETURN':
            if len(call_stack) == 0:
                exit(56)
            else:
                pc = call_stack.pop()
                continue

        case 'BREAK':
            print(f"Current instruction = {instruction.name}")
            print(f"The number of executed instructions = {str(instruction_count)}")
            frame_stack.print_frames()

        #<INSTRUCTION> <label>
        case 'CALL':
            label = instruction.get_argument(0)
            if label.value not in labels:
                exit(52)
            else:
                call_stack.append(pc + 1)
                pc = labels[label.value]
                continue

        case 'LABEL':
            pass

        case 'JUMP':
            label = instruction.get_argument(0)
            if label.value not in labels:
                exit(52)
            else:
                pc = labels[label.value]
                continue

        #<INSTRUCTION> <var>
        case 'DEFVAR':
            var = instruction.get_argument(0)
            name = var.value
            frame_stack.create_variable(name, "")

        case 'POPS':
            var = instruction.get_argument(0)
            if len(data_stack) == 0:
                exit(56)
            value = data_stack.pop()
            frame_stack.set_variable(var.value, value)

        #<INSTRUCTION> <symb>
        case 'PUSHS':
            symb = instruction.get_argument(0)
            if symb.type == "var":
                data_stack.append(frame_stack.get_variable(symb.value))
            elif symb.type == "int":
                data_stack.append(toInt(symb.value))
            elif symb.type == "bool":
                data_stack.append(toBool(symb.value))
            elif symb.type == "string":
                data_stack.append(toStr(symb.value))
            elif symb.type == "nil":
                data_stack.append(None)

        case 'WRITE':
            symb = instruction.get_argument(0)
            if symb.type == "var":
                to_print = frame_stack.get_variable(symb.value)
                if to_print is None:
                    print("", end='')
                elif to_print is True:
                    print("true", end='')
                elif to_print is False:
                    print("false", end='')
                else:
                    print(to_print, end='')
            elif symb.type == "int":
                print(toInt(symb.value), end='')
            elif symb.type == "bool":
                print(toBool(symb.value), end='')
            elif symb.type == "string":
                print(toStr(symb.value), end='')
            elif symb.type == "nil":
                print("", end='')

        case 'EXIT':
            symb = instruction.get_argument(0)
            
            if symb.type == "var":
                exit_code = frame_stack.get_variable(symb.value)
                if getType(exit_code) != "int":
                    exit(53)
            elif symb.type == "int":
                exit_code = toInt(symb.value)
            else:
                exit(53)

            if exit_code < 0 or 49 < exit_code:
                exit(57)
            else:
                exit(exit_code)

        case 'DPRINT':            
            symb = instruction.get_argument(0)
            if symb.type == "var":
                to_print = frame_stack.get_variable(symb.value)
                if to_print is None:
                    print("", file=sys.stderr, end='')
                elif to_print is True:
                    print("true", file=sys.stderr, end='')
                elif to_print is False:
                    print("false", file=sys.stderr, end='')
                else:
                    print(to_print, file=sys.stderr, end='')
            elif symb.type == "int":
                print(symb.value, file=sys.stderr, end = '')
            elif symb.type == "bool":
                print(symb.value, file=sys.stderr, end = '')
            elif symb.type == "string":
                print(symb.value, file=sys.stderr, end = '')
            elif symb.type == "nil":
                print("", file=sys.stderr, end = '')

        #<INSTRUCTION> <var> <symb>
        case 'MOVE':
            var = instruction.get_argument(0)
            symb = instruction.get_argument(1)
            if symb.type == "var":
                value = frame_stack.get_variable(symb.value)
                frame_stack.set_variable(var.value, value)
            elif symb.type == "int":
                value = toInt(symb.value)
                frame_stack.set_variable(var.value, value)
            elif symb.type == "bool":
                value = toBool(symb.value)
                frame_stack.set_variable(var.value, value)
            elif symb.type == "string":
                value = symb.value
                frame_stack.set_variable(var.value, value)
            elif symb.type == "nil":
                value = None
                frame_stack.set_variable(var.value, value)

        case 'INT2CHAR':
            var = instruction.get_argument(0)
            symb = instruction.get_argument(1)
            if symb.type == "var":
                ascii_val = frame_stack.get_variable(symb.value)
                if getType(ascii_val) != "int":
                    exit(53)
            elif symb.type == "int":
                ascii_val = toInt(symb.value)
            else:
                exit(53)
            if ascii_val is None:
                exit(53)
            if ascii_val < 0 or ascii_val > 1_114_111:
                exit(58)
            frame_stack.set_variable(var.value, chr(ascii_val))

        case 'STRLEN':
            var = instruction.get_argument(0)
            symb = instruction.get_argument(1)
            if symb.type == "var":
                if getType(frame_stack.get_variable(symb.value)) == "str":
                    len = len(frame_stack.get_variable(symb.value))
                    frame_stack.set_variable(var.value, len)
                else:
                    exit(53)
            elif symb.type == "string":
                if symb.value is None:
                    frame_stack.set_variable(var.value, 0)
                else:
                    frame_stack.set_variable(var.value, len(symb.value))
            else:
                exit(53)

        case 'TYPE':
            var = instruction.get_argument(0)
            symb = instruction.get_argument(1)
            if symb.type == "var":
                type = getType(frame_stack.get_variable(symb.value))
                frame_stack.set_variable(var.value, type)
            elif symb.type == "int":
                frame_stack.set_variable(var.value, "int")
            elif symb.type == "bool":
                frame_stack.set_variable(var.value, "bool")
            elif symb.type == "string":
                frame_stack.set_variable(var.value, "string")
            elif symb.type == "nil":
                frame_stack.set_variable(var.value, "nil")

        case 'NOT':
            var = instruction.get_argument(0)
            symb = instruction.get_argument(1)
            if symb.type == "var":
                value = frame_stack.get_variable(symb.value)
                if getType(value) != "bool":
                    exit(53)
                else:
                    frame_stack.set_variable(var.value, not value)
            elif symb.type == "bool":
                if symb.value == "true":
                    frame_stack.set_variable(var.value, False)
                elif symb.value == "false":
                    frame_stack.set_variable(var.value, True)
            else:
                exit(53)


        #<INSTRUCTION> <var> <type>
        case 'READ':
            var = instruction.get_argument(0)
            type = instruction.get_argument(1)

            value = input.readline().strip()
            if type.value == "int":
                value = toInt(value)
            elif type.value == "string":
                value = toStr(value)
            elif type.value == "bool":
                value = toBool(value)
            elif type.value == "nil":
                value = None
            frame_stack.set_variable(var.value, value)

        #<INSTRUCTION> <var> <symb1> <symb2>
        case 'ADD':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if var.type != "var":
                exit(53)

            if symb1.type == "var":
                add1 = frame_stack.get_variable(symb1.value)
                if not isinstance(add1, int):
                    exit(53)
            elif symb1.type == "int":
                add1 = toInt(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                add2 = frame_stack.get_variable(symb2.value)
                if not isinstance(add2, int):
                    exit(53)
            elif symb2.type == "int":
                add2 = toInt(symb2.value)
            else:
                exit(53)

            frame_stack.set_variable(var.value, add1 + add2)

        case 'SUB':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if var.type != "var":
                exit(53)

            if symb1.type == "var":
                sub1 = frame_stack.get_variable(symb1.value)
                if not isinstance(sub1, int):
                    exit(53)
            elif symb1.type == "int":
                sub1 = toInt(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                sub2 = frame_stack.get_variable(symb2.value)
                if not isinstance(sub2, int):
                    exit(53)
            elif symb2.type == "int":
                sub2 = toInt(symb2.value)
            else:
                exit(53)

            frame_stack.set_variable(var.value, sub1 - sub2)

        case 'MUL':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if var.type != "var":
                exit(53)

            if symb1.type == "var":
                mul1 = frame_stack.get_variable(symb1.value)
                if not isinstance(mul1, int):
                    exit(53)
            elif symb1.type == "int":
                mul1 = toInt(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                mul2 = frame_stack.get_variable(symb2.value)
                if not isinstance(mul2, int):
                    exit(53)
            elif symb2.type == "int":
                mul2 = toInt(symb2.value)
            else:
                exit(53)

            frame_stack.set_variable(var.value, mul1 * mul2)

        case 'IDIV':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if var.type != "var":
                exit(53)

            if symb1.type == "var":
                idiv1 = frame_stack.get_variable(symb1.value)
                if not isinstance(idiv1, int):
                    exit(53)
            elif symb1.type == "int":
                idiv1 = toInt(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                idiv2 = frame_stack.get_variable(symb2.value)
                if not isinstance(idiv2, int):
                    exit(53)
            elif symb2.type == "int":
                idiv2 = toInt(symb2.value)
            else:
                exit(53)

            if idiv2 == 0:
                exit(57)

            frame_stack.set_variable(var.value, idiv1 // idiv2)

        case 'LT':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
            elif symb1.type == "int":
                value1 = toInt(symb1.value)
            elif symb1.type == "bool":
                value1 = toBool(symb1.value)
            elif symb1.type == "string":
                value1 = toStr(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
            elif symb2.type == "int":
                value2 = toInt(symb2.value)
            elif symb2.type == "bool":
                value2 = toBool(symb2.value)
            elif symb2.type == "string":
                value2 = toStr(symb2.value)
            else:
                exit(53)

            if type(value1) == type(value2):
                frame_stack.set_variable(var.value, value1 < value2)
            else:
                exit(53)

        case 'GT':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
            elif symb1.type == "int":
                value1 = toInt(symb1.value)
            elif symb1.type == "bool":
                value1 = toBool(symb1.value)
            elif symb1.type == "string":
                value1 = toStr(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
            elif symb2.type == "int":
                value2 = toInt(symb2.value)
            elif symb2.type == "bool":
                value2 = toBool(symb2.value)
            elif symb2.type == "string":
                value2 = toStr(symb2.value)
            else:
                exit(53)

            if type(value1) == type(value2):
                frame_stack.set_variable(var.value, value1 > value2)
            else:
                exit(53)

        case 'EQ':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
            elif symb1.type == "int":
                value1 = toInt(symb1.value)
            elif symb1.type == "bool":
                value1 = toBool(symb1.value)
            elif symb1.type == "string":
                value1 = toStr(symb1.value)
            elif symb1.type == "nil":
                value1 = None

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
            elif symb2.type == "int":
                value2 = toInt(symb2.value)
            elif symb2.type == "bool":
                value2 = toBool(symb2.value)
            elif symb2.type == "string":
                value2 = toStr(symb2.value)
            elif symb1.type == "nil":
                value2 = None

            if value1 is None or value2 is None or type(value1) == type(value2):
                frame_stack.set_variable(var.value, value1 == value2)
            else:
                exit(53)

        case 'AND':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
                type = getType(value1)
                if type != "bool":
                    exit(53)
            elif symb1.type == "bool":
                if symb1.value == "true":
                    value1 = True
                else:
                    value1 = False
            else:
                exit(53)

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
                type = getType(value2)
                if type != "bool":
                    exit(53)
            elif symb2.type == "bool":
                if symb2.value == "true":
                    value2 = True
                else:
                    value2 = False
            else:
                exit(53)

            frame_stack.set_variable(var.value, value1 and value2)

        case 'OR':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
                type = getType(value1)
                if type != "bool":
                    exit(53)
            elif symb1.type == "bool":
                if symb1.value == "true":
                    value1 = True
                else:
                    value1 = False
            else:
                exit(53)

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
                type = getType(value2)
                if type != "bool":
                    exit(53)
            elif symb2.type == "bool":
                if symb2.value == "true":
                    value2 = True
                else:
                    value2 = False
            else:
                exit(53)

            frame_stack.set_variable(var.value, value1 or value2)

        case 'STRI2INT':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                string = frame_stack.get_variable(symb1.value)
                if getType(string) != "string":
                    exit(53)
            elif symb1.type == "string":
                string = symb1.value
            else:
                exit(53)

            if symb2.type == "var":
                index = frame_stack.get_variable(symb2.value)
                if getType(index) != "int":
                    exit(53)
            elif symb2.type == "int":
                index = toInt(symb2.value)
            else:
                exit(53)

            if index >= len(string) or index < 0:
                exit(58)

            frame_stack.set_variable(var.value, ord(string[index]))

        case 'CONCAT':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                string1 = frame_stack.get_variable(symb1.value)
                if getType(string1) != "string":
                    exit(53)
            elif symb1.type == "string":
                string1 = symb1.value
            else:
                exit(53)

            if symb2.type == "var":
                string2 = frame_stack.get_variable(symb2.value)
                if getType(string2) != "string":
                    exit(53)
            elif symb2.type == "string":
                string2 = symb2.value
            else:
                exit(53)
            if string1 is None:
                string1 = ""
            if string2 is None:
                string2 = ""
            frame_stack.set_variable(var.value, string1 + string2)

        case 'GETCHAR':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1) #str
            symb2 = instruction.get_argument(2) #int

            if symb1.type == "var":
                string = frame_stack.get_variable(symb1.value)
                if getType(string) != "string":
                    exit(53)
            elif symb1.type == "string":
                string = symb1.value
            else:
                exit(53)

            if symb2.type == "var":
                index = frame_stack.get_variable(symb2.value)
                if getType(index) != "int":
                    exit(53)
            elif symb2.type == "int":
                index = toInt(symb2.value)
            else:
                exit(53)
            if index >= len(string) or index < 0:
                exit(58)
            frame_stack.set_variable(var.value, string[index])

        case 'SETCHAR':
            var = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                index = frame_stack.get_variable(symb1.value)
                if getType(index) != "int":
                    exit(53)
            elif symb1.type == "int":
                index = toInt(symb1.value)
            else:
                exit(53)

            if symb2.type == "var":
                char = frame_stack.get_variable(symb2.value)
                if getType(char) != "string":
                    exit(53)
            elif symb2.type == "string":
                char = symb2.value
            else:
                exit(53)
            if char is None:
                exit(58)
            char = char[0]

            string = frame_stack.get_variable(var.value)
            if getType(string) != "string":
                exit(53)
            if index >= len(string) or index < 0:
                exit(58)
            string = string[:index] + char + string[index+1:]
                            
            frame_stack.set_variable(var.value, string)

        #<INSTRUCTION> <label> <symb1> <symb2>
        case 'JUMPIFEQ':
            label = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
            elif symb1.type == "int":
                value1 = toInt(symb1.value)
            elif symb1.type == "bool":
                value1 = toBool(symb1.value)
            elif symb1.type == "string":
                value1 = toStr(symb1.value)
            elif symb1.type == "nil":
                value1 = None

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
            elif symb2.type == "int":
                value2 = toInt(symb2.value)
            elif symb2.type == "bool":
                value2 = toBool(symb2.value)
            elif symb2.type == "string":
                value2 = toStr(symb2.value)
            elif symb2.type == "nil":
                value2 = None

            if label.value not in labels:
                exit(52)
            elif value1 is None or value2 is None or type(value1) == type(value2):
                if value1 == value2:
                    pc = labels[label.value]
                    continue
            else:
                exit(53)
        case 'JUMPIFNEQ':
            label = instruction.get_argument(0)
            symb1 = instruction.get_argument(1)
            symb2 = instruction.get_argument(2)

            if symb1.type == "var":
                value1 = frame_stack.get_variable(symb1.value)
            elif symb1.type == "int":
                value1 = toInt(symb1.value)
            elif symb1.type == "bool":
                value1 = toBool(symb1.value)
            elif symb1.type == "string":
                value1 = toStr(symb1.value)
            elif symb1.type == "nil":
                value1 = None

            if symb2.type == "var":
                value2 = frame_stack.get_variable(symb2.value)
            elif symb2.type == "int":
                value2 = toInt(symb2.value)
            elif symb2.type == "bool":
                value2 = toBool(symb2.value)
            elif symb2.type == "string":
                value2 = toStr(symb2.value)
            elif symb2.type == "nil":
                value2 = None

            if label.value not in labels:
                exit(52)
            elif value1 is None or value2 is None or type(value1) == type(value2):
                if value1 != value2:
                    pc = labels[label.value]
                    continue
            else:
                exit(53)
    pc += 1

if args['source'] is not None:
    source.close()
if args['input'] is not None:
    input.close()
from math_functions import *
import tkinter

#create variables for gui
default_display_value = "0.0"
display_content = default_display_value
max_display_length = 21
answer_displayed = False

#create variables used for calculations
operation  = None
operand1 = None
operand2 = default_display_value

#If theres a value in nested array returns index, else returns None
def find_value_in_nested_array(value, array):
    for index, nested_array in enumerate(array):
        if value in nested_array:
            return index
    return None

#returns True if char is number, else False
def is_number(symbol):
    if len(symbol) == 1:
        if 48 <= ord(symbol) and ord(symbol) <= 57:
            return True
    return False

#if clear flag is true it clears display_content. Appends to char display_content
def display_add(clear, digit):
    global display_content

    if clear == True or display_content == default_display_value: #clears if flag or there's default_display_value
        display_content = ""
    display_content += digit #appends the digit at the end of string
    if len(display_content) >= max_display_length: #checks if there's enough space for another char in display
        display_content = "Error"
    canvas.delete("displayed")
    canvas.create_text(390, 55,text = display_content,font="Times 25", anchor="e", tag = "displayed") #update display content

#digits are addet to display and functions are saved
def handle_symbol(symbol):
    global display_content, operation, operand1, operand2, answer_displayed

    #if number is pressed and there is enough space in display
    if is_number(symbol) and len(display_content) <= max_display_length:
        if answer_displayed == False: #if answer is not displayed it will not clear display
            display_add(False, symbol)
        else:
            display_add(True, symbol) #if answer is displayed it will clear display
            answer_displayed = False
        operand2 = display_content #update operand2
    #if anything else is pressed
    else:
        index = find_value_in_nested_array(symbol, buttons) #finds which function has been pressed in buttons
        if index != None:
            if buttons[index][3] == equals: #special func equals
                equals()
            elif buttons[index][3] == clear_operands: #special func clear_operands
                clear_operands()
            else:
                operation = buttons[index][3] #save the address of operation
                if operand2 != None: #this is fix for two functions pressed in a row
                    operand1 = operand2 #move operand2 to operand1
                    operand2 = None #clear operand2
                    display_content = default_display_value

#handles input by mouse
def mouse_clicked(event):
    x = (event.x - 5*2) // 100 #calculates column index
    y = (event.y - 10*2 - 90) // 100 #calculates row index
    index = x + (4 * y) #calculates button pressed index in buttons array
    if 0 <= index and index <= 19: #if it's valid buttons array index handle_symbol is being called
        handle_symbol(buttons[index][1])

#handles input by keyboard
def key_pressed(event):
    if event.keysym == "Return": #bind for enter to call equals
        handle_symbol("=")
    elif event.keysym == "BackSpace": #bind backspace to call clear_operands
        handle_symbol("C")
    elif event.char == "/": #fix: division is / on keyboard insted of ÷
        handle_symbol("÷")
    elif event.keysym != "equal" and event.char != "C": #passes the symbol of pressed func
        handle_symbol(event.char)

#binds input
def binds():
    #bind keyboard and left mouse button
    canvas.bind('<Button-1>', mouse_clicked)
    canvas.bind_all("<Key>", key_pressed)

#create gui
def gui_ctor():
    global canvas, buttons

    #create canvas
    canvas_width = 410
    canvas_height = 615
    canvas = tkinter.Canvas(width = canvas_width, height = canvas_height, background='lightblue')
    canvas.pack()

    #create display
    outline_width = 2
    display = (5,10,405,100)
    canvas.create_rectangle(display, width = outline_width, fill = "white")
    #create buttons
    buttons = (
        ((5,110,105,210), "xⁿ ", "lightgrey", exponentiate),
        ((105,110,205,210), "ⁿ√x", "lightgrey", nthroot),
        ((205,110,305,210), "|x|", "lightgrey", absolute),
        ((305,110,405,210), "C", "snow4", clear_operands),

        ((5,210,105,310), "7", "white", None),
        ((105,210,205,310), "8", "white", None),
        ((205,210,305,310), "9", "white", None),
        ((305,210,405,310), "!", "lightgrey", factorial),

        ((5,310,105,410), "4", "white", None),
        ((105,310,205,410), "5", "white", None),
        ((205,310,305,410), "6", "white", None),
        ((305,310,405,410), "÷", "lightgrey", divide),

        ((5,410,105,510), "1", "white", None),
        ((105,410,205,510), "2", "white", None),
        ((205,410,305,510), "3", "white", None),
        ((305,410,405,510), "*", "lightgrey", multiply),

        ((5,510,105,610), "0", "white", None),
        ((105,510,205,610), "-", "lightgrey", subtract),
        ((205,510,305,610), "+", "lightgrey", addition),
        ((305,510,405,610), "=", "tomato", equals)
    )

    for button in buttons:
        coordinations = button[0]
        button_color = button[2]

        canvas.create_rectangle(coordinations, fill = button_color, width = outline_width, outline = 'black')

        text_x = (coordinations[0]+coordinations[2])//2
        text_y = (coordinations[1]+coordinations[3])//2
        button_text = button[1]
        canvas.create_text(text_x, text_y, text = button_text, font="Times 20 bold")
    #set default_display_value fromt the start of app
    display_add(True, default_display_value)

#call operation function with either operand1 and operand2 or only operand1
def calculate(operation, operand1, operand2):
    result = None

    if operand1 != None:
        operand1 = float(operand1) #change type to float
    if operand2 != None:
        operand2 = float(operand2) #change type to float
    
    if operation != absolute and operation != factorial: #calls binary operations
        if operand1 != None and operand2 != None:
            result = operation(operand1, operand2)
    else: #calls unary operations
        if operand1 != None:
            result = operation(operand1)
    
    return result

#after enter/= has been pressed equals calls calculate and  calculates result
def equals():
    global operation, operand1, operand2, answer_displayed

    result = calculate(operation, operand1, operand2)
    if result != None:
        display_add(True, str(result)) #clear display and show result
        answer_displayed = True
    if operand1 != None: 
        operand2 = result #saves result to operand 2 so it can bed used for another operation
        operand1 = None

#clears operands and operations
def clear_operands():
    global operand1, operand2, operation

    operand1 = None
    operand2 = None
    operation = None
    display_add(True, default_display_value) #clear default_display_value

gui_ctor()
binds()

canvas.mainloop()
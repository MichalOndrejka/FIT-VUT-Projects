<?php
ini_set('display_errors', 'stderr');

function removeComment($code) {
    $index = strpos($code, "#");
    if ($index === 0) return "";
    elseif ($index === FALSE) return trim($code, " \n");
    else return trim(substr($code, 0, $index), " \n");
}

function printVar($var, $index) {
    if (preg_match("/^(GF|LF|TF)@([a-zA-Z]|_|-|\\$|&|%|\\*|!|\\?)([a-zA-Z0-9]|_|-|\\$|&|%|\\*|!|\\?)*$/", $var)) {
        $var = htmlspecialchars($var);
        echo("\t\t<arg".$index." type=\"var\">".$var."</arg".$index.">\n");
    } else {
        echo("Invalid variable\n");
        exit(23);
    }
}

function printSymb($symb, $index) {
    if (preg_match("/^(GF|LF|TF)@([a-zA-Z]|_|-|\\$|&|%|\\*|!|\\?)([a-zA-Z0-9]|_|-|\\$|&|%|\\*|!|\\?)*$/", $symb)) {
        echo("\t\t<arg".$index." type=\"var\">".$symb."</arg".$index.">\n");
        return;
    }
    $type = explode("@" ,trim($symb, " \n"))[0];
    switch ($type) {
        case "int":
            $symb = substr($symb, 4);
            if (preg_match("/^(-|\\+)?[0-9]+$/", $symb) == 0) {
                echo("Invalid integer\n");
                exit(23);
            }
            break;
        case "bool":
            $symb = substr($symb, 5);
            if (preg_match("/^((true)|(false))$/", $symb) == 0) {
                echo("Invalid bool\n");
                exit(23);
            }
            break;
        case "string":
            $symb = substr($symb, 7);
            if (preg_match("/^([^#\\\\]|(\\\\[0-9]{3}))*$/", $symb) == 0) {
                echo("Invalid string\n");
                exit(23);
            }
            break;
        case "nil":
            $symb = substr($symb, 4);
            if (preg_match("/^(nil)$/", $symb) == 0) {
                echo("Invalid nil\n");
                exit(23);
            }
            break;
        default:
            echo("Invalid type\n");
            exit(23);
            break;
    }
    echo("\t\t<arg".$index." type=\"".$type."\">".$symb."</arg".$index.">\n");
}

function printLabel($label, $index) {
    if (preg_match("/^([a-zA-Z0-9]|_|-|\\$|&|%|\\*|!|\\?)+$/", $label) == 0) {
        echo("Invalid label\n");
        exit(23);
    }
    echo("\t\t<arg".$index." type=\"label\">".$label."</arg".$index.">\n");
}

function printType($label, $index) {
    if (preg_match("/^((int)|(bool)|(string))$/", $label) == 1) {
        echo("\t\t<arg".$index." type=\"type\">".$label."</arg".$index.">\n");
    } else {
        echo("Invalid type\n");
        exit(23);
    }
}

function checkCount($splitted_line, $count) {
    if (count($splitted_line) != $count) {
        echo("Invalid argument count\n");
        exit(23);
    }
}

if ($argc == 2) {
    if ($argv[1] == "--help") {
        echo("Usage: php parser.php [options] < inputFile\n");
        exit(0);
    } else {
        echo("Invalid --help\n");
        exit(10);
    }
} elseif ($argc > 2) {
    echo("Invalid argument count\n");
    exit(23);
}

do {
    $line = fgets(STDIN);
    $line = removeComment($line);
} while ($line == "");
if ($line != ".IPPcode23") {
    echo("Invalid header\n");
    exit(21);
}
echo("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
echo("<program language=\"IPPcode23\">\n");

$order = 1;

while ($line = fgets(STDIN))
{
    $line = removeComment($line);
    if ($line === ""){
        continue;
    }
    $splitted_line = preg_split("/\\s+/", $line);
    $instruction = strtoupper($splitted_line[0]);
    echo("\t<instruction order=\"".$order++."\" opcode=\"".$instruction."\">\n");
    switch($instruction)
    {
        //<INSTRUCTION>
        case 'CREATEFRAME':
            checkCount($splitted_line, 1);
            break;

        case 'PUSHFRAME':
            checkCount($splitted_line, 1);
            break;

        case 'POPFRAME':
            checkCount($splitted_line, 1);
            break;

        case 'RETURN':
            checkCount($splitted_line, 1);
            break;

        case 'BREAK':
            checkCount($splitted_line, 1);
            break;

        //<INSTRUCTION> <label>
        case 'CALL':
            checkCount($splitted_line, 2);
            printLabel($splitted_line[1], 1);
            break;

        case 'LABEL':
            checkCount($splitted_line, 2);
            printLabel($splitted_line[1], 1);
            break;

        case 'JUMP':
            checkCount($splitted_line, 2);
            printLabel($splitted_line[1], 1);
            break;

        //<INSTRUCTION> <var>
        case 'DEFVAR':
            checkCount($splitted_line, 2);
            printVar($splitted_line[1], 1);
            break;

        case 'CALL':
            checkCount($splitted_line, 2);
            printVar($splitted_line[1], 1);
            break;

        case 'POPS':
            checkCount($splitted_line, 2);
            printVar($splitted_line[1], 1);
            break;

        //<INSTRUCTION> <symb>
        case 'PUSHS':
            checkCount($splitted_line, 2);
            printSymb($splitted_line[1], 1);
            break;

        case 'WRITE':
            checkCount($splitted_line, 2);
            printSymb($splitted_line[1], 1);
            break;

        case 'EXIT':
            checkCount($splitted_line, 2);
            printSymb($splitted_line[1], 1);
            break;

        case 'DPRINT':
            checkCount($splitted_line, 2);
            printSymb($splitted_line[1], 1);
            break;

        //<INSTRUCTION> <var> <symb>
        case 'MOVE':
            checkCount($splitted_line, 3);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            break;

        case 'INT2CHAR':
            checkCount($splitted_line, 3);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            break;

        case 'STRLEN':
            checkCount($splitted_line, 3);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            break;

        case 'TYPE':
            checkCount($splitted_line, 3);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            break;

        case 'NOT':
            checkCount($splitted_line, 3);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            break;

        //<INSTRUCTION> <var> <type>
        case 'READ':
            checkCount($splitted_line, 3);
            printVar($splitted_line[1], 1);
            printType($splitted_line[2], 2);
            break;

        //<INSTRUCTION> <var> <symb1> <symb2>
        case 'ADD':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'SUB':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'MUL':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'IDIV':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'LT':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'GT':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'EQ':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'AND':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'OR':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'STRI2INT':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'CONCAT':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'GETCHAR':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'SETCHAR':
            checkCount($splitted_line, 4);
            printVar($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        //<INSTRUCTION> <label> <symb1> <symb2>
        case 'JUMPIFEQ':
            checkCount($splitted_line, 4);
            printLabel($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        case 'JUMPIFNEQ':
            checkCount($splitted_line, 4);
            printLabel($splitted_line[1], 1);
            printSymb($splitted_line[2], 2);
            printSymb($splitted_line[3], 3);
            break;

        default:
            echo("Invalid instruction\n");
            exit(22);
            break;
    }
    echo("\t</instruction>\n");
}
echo("</program>\n")
?>
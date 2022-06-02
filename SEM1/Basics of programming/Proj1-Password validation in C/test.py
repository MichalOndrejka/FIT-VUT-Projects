#!/usr/bin/python3
#
# Testy na 1. IZP projekt 2021
# Vytvoril Josef Kuchar (xkucha28) - josefkuchar.com
# Priklad pouziti: ./test.py pwcheck
# Help se vypise pres argument -h
# Revize: 7

from subprocess import run, PIPE
import sys
import argparse

OK = "\033[1;32;40m[ OK ]\033[0;37;40m"
FAIL = "\033[1;31;40m[FAIL]\033[0;37;40m"

class Tester:
    def __init__(self, program_name):
        self.program_name = './' + program_name
        self.test_count = 0
        self.pass_count = 0

    def test(self, test_name, args, input, output='', intentional_error=False):
        self.test_count += 1
        error = False
        msg = ""
        p = None

        try:
            p = run([self.program_name] + args, stdout=PIPE, stderr=PIPE,
                    input=input, encoding='ascii')
        except UnicodeDecodeError as e:
            print(FAIL, test_name)
            print('Vystup pravdepodobne obsahuje znaky mimo ASCII (diakritika?)')
            print(e)
            sys.exit()
        except Exception as e:
            print(FAIL, test_name)
            print('Chyba pri volani programu!')
            print(e)
            sys.exit()

        if p.returncode != 0:
            if not intentional_error:
                error = True
                msg += 'Program vratil chybovy navratovy kod ({}) prestoze nemel!\n'.format(p.returncode)
        else:
            if intentional_error:
                error = True
                msg += 'Program vratil uspesne dokonceni (kod 0) prestoze nemel!\n'

        if output.rstrip() != p.stdout.rstrip():
            error = True
            msg += 'Vystup programu se neshoduje s predpokladanym vystupem!\n'

        if intentional_error and p.stderr == '':
            error = True
            msg = 'Program nevratil na STDERR zadnou chybovou zpravu!\n'

        if error:
            print(FAIL, test_name)
            print(msg)
            print('Argumenty: ', ' '.join(args))
            print("Predpokladany vystup:")
            print(output)
            print("STDOUT:")
            print(p.stdout)
            print("STDERR:")
            print(p.stderr)
        else:
            self.pass_count += 1
            print(OK, test_name)

    def print_stats(self):
        print('Uspesnost: {}/{} ({:.2f}%)'.format(self.pass_count, self.test_count, (self.pass_count / self.test_count) * 100))
        pass

HESLA_TXT = '1234567890\nPassword\nHeslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n'
HESLA_TXT_CRLF = '1234567890\r\nPassword\r\nHeslo123\r\nMojevelmidlouhehesloscislem0\r\nIZP2021:fit@vut.cz\r\n'

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Tester 1. IZP projektu')
    parser.add_argument('prog', metavar='P', type=str, help='Jmeno programu (napriklad: pwcheck)')
    parser.add_argument('--bonus', dest='bonus', action='store_true', help='Kontrola bonusoveho parsovani argumentu')
    parser.add_argument('--crlf', dest='crlf', action='store_true', help='Kontrola podpory CRLF vstupu')
    parser.add_argument('--no-color', dest='color', action='store_false', help='Vystup bez barev')
    args = parser.parse_args()

    if not args.color:
        OK = '[ OK ]'
        FAIL = '[FAIL]'

    t1 = Tester(args.prog)
    t2 = Tester(args.prog)
    t3 = Tester(args.prog)

    # Testy ze zadani
    t1.test('Test ze zadani #1', ['1', '1'], HESLA_TXT, 'Password\nHeslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n')
    t1.test('Test ze zadani #2', ['2', '3'], HESLA_TXT, 'Heslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n')
    t1.test('Test ze zadani #3', ['3', '2'], HESLA_TXT, 'Heslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n'),
    t1.test('Test ze zadani #4', ['4', '2'], HESLA_TXT, 'Heslo123\nIZP2021:fit@vut.cz\n'),
    t1.test('Test ze zadani #5', ['2', '4', '--stats'], HESLA_TXT, 'IZP2021:fit@vut.cz\nStatistika:\nRuznych znaku: 36\nMinimalni delka: 8\nPrumerna delka: 14.4\n')
    # Vlastni testy pravidel
    t1.test('Test na LEVEL 1 #1', ['1', '1'], 'Aa\n', 'Aa\n')
    t1.test('Test na LEVEL 1 #2', ['1', '1'], 'Zz\n', 'Zz\n')
    t1.test('Test na LEVEL 1 #3', ['1', '1'], 'aaa\n', '')
    t1.test('Test na LEVEL 1 #4', ['1', '1'], 'A0\n', '')
    t1.test('Test na LEVEL 2 #1', ['2', '1'], 'aaaaa\n', '')
    t1.test('Test na LEVEL 2 #2', ['2', '2'], 'Aa\n', 'Aa\n')
    t1.test('Test na LEVEL 2 #3', ['2', '2'], 'Zz\n', 'Zz\n')
    t1.test('Test na LEVEL 2 #4', ['2', '4'], '%aZ0\n', '%aZ0\n')
    t1.test('Test na LEVEL 2 #5', ['2', '5'], '%aZ0\n', '%aZ0\n')
    t1.test('Test na LEVEL 2 #6', ['2', '4'], 'aZ0\n', '')
    t1.test('Test na LEVEL 2 #7', ['2', '3'], 'aZ0\n', 'aZ0\n')
    t1.test('Test na LEVEL 2 #8', ['2', '4'], 'Aa 0\n', 'Aa 0\n')
    t1.test('Test na LEVEL 3 #1', ['3', '1'], 'Aa\n', '')
    t1.test('Test na LEVEL 3 #2', ['3', '2'], 'Aa\n', 'Aa\n')
    t1.test('Test na LEVEL 3 #3', ['3', '2'], 'AAa\n', '')
    t1.test('Test na LEVEL 3 #4', ['3', '4'], 'Az0!!!\n', 'Az0!!!\n')
    t1.test('Test na LEVEL 3 #5', ['3', '4'], 'Az0!!!!\n', '')
    t1.test('Test na LEVEL 3 #6', ['3', '5'], 'Az0!!!!\n', 'Az0!!!!\n')
    t1.test('Test na LEVEL 3 #7', ['3', '5'], 'Az0!!!!!\n', '')
    t1.test('Test na LEVEL 4 #1', ['4', '1'], 'Aa\n', '')
    t1.test('Test na LEVEL 4 #2', ['4', '2'], 'Aaha\n', 'Aaha\n')
    t1.test('Test na LEVEL 4 #3', ['4', '2'], 'Abhaha\n', '')
    t1.test('Test na LEVEL 4 #4', ['4', '3'], '0Abhaha\n', '0Abhaha\n')
    t1.test('Test na LEVEL 4 #5', ['4', '3'], '0Abx0Abx\n', '')
    t1.test('Test na LEVEL 4 #6', ['4', '3'], '0Ahahah\n', '')
    t1.test('Test na LEVEL 4 #7', ['4', '4'], '%aZ0x%aZ\n', '%aZ0x%aZ\n')
    t1.test('Test na LEVEL 4 #8', ['4', '4'], '%aZ0x%aZ0\n', '')
    t1.test('Test na LEVEL 4 #9', ['4', '4'], '%aZ0hahaha\n', '')
    t1.test('Test na LEVEL 4 #10', ['4', '5'], '%aZ0hahaha\n', '%aZ0hahaha\n')
    t1.test('Test na LEVEL 4 #11', ['4', '5'], '%aZ0hahahah\n', '')
    t1.test('Test na LEVEL 4 #12', ['4', '5'], 'hahah%aZ0hahah\n', '')
    # Pocet argumentu
    t1.test('Nedostatecny pocet argumentu', ['1'], HESLA_TXT, intentional_error=True)
    t1.test('Prilis mnoho argumentu', ['1', '1', '--stats', '1'], HESLA_TXT, intentional_error=True)
    # Parsovani cisel v argumentech
    t1.test('Ciselne argumenty maji pismena na konci', ['1a', '1a'], HESLA_TXT, intentional_error=True)
    t1.test('Cislene argumenty maji pismena na zacatku', ['a1', 'a1'], HESLA_TXT, intentional_error=True)
    t1.test('Cislene argumenty maji pismena uprostred', ['0a1', '0a1'], HESLA_TXT, intentional_error=True)
    # Testovani rozsahu prvniho argumentu
    t1.test('Prvni argument mimo rozsah #1: 0', ['0', '1'], HESLA_TXT, intentional_error=True)
    t1.test('Prvni argument mimo rozsah #2: 5', ['5', '1'], HESLA_TXT, intentional_error=True)
    t1.test('Prvni argument mimo rozsah #3: 2000000000', ['2000000000', '1'], HESLA_TXT, intentional_error=True)
    t1.test('Prvni argument mimo rozsah #4: vetsi nez LONG_MAX', ['10223372036854775807', '1'], HESLA_TXT, intentional_error=True)
    t1.test('Prvni argument mimo rozsah #5: -2000000000', ['-2000000000', '1'], HESLA_TXT, intentional_error=True)
    t1.test('Prvni argument mimo rozsah #6: mensi nez LONG_MIN', ['-10223372036854775807', '1'], HESLA_TXT, intentional_error=True)
    # Testovani rozsahu druheho argumentu
    t1.test('Druhy argument mimo rozsah #1: 0', ['1', '0'], HESLA_TXT, intentional_error=True)
    t1.test('Druhy argument mimo rozsah #2: -2000000000', ['1', '-2000000000'], HESLA_TXT, intentional_error=True)
    t1.test('Druhy argument mimo rozsah #3: mensi nez LONG_MIN', ['1', '-10223372036854775807'], HESLA_TXT, intentional_error=True)
    t1.test('Druhy argument velky: vetsi nez LONG_MAX', ['4', '10223372036854775807'], HESLA_TXT, 'IZP2021:fit@vut.cz\n')
    # Testovani parsovani tretiho argumentu
    t1.test('Treti argument (--stats) neplatny #1: --haha', ['1', '1', '--haha'], HESLA_TXT, intentional_error=True)
    t1.test('Treti argument (--stats) neplatny #2: --statshaha', ['1', '1', '--statshaha'], HESLA_TXT, intentional_error=True)
    t1.test('Treti argument (--stats) neplatny #3: stats', ['1', '1', '--statshaha'], HESLA_TXT, intentional_error=True)
    # Testovani ruznych moznosti vstupu
    t1.test('Maximalni delka hesla', ['1', '1', '--stats'], 'A' * 100 +'\n', 'Statistika:\nRuznych znaku: 1\nMinimalni delka: 100\nPrumerna delka: 100.0\n')
    t1.test('Heslo prilis dlouhe #1: 101', ['1', '1'], 'A' * 101 + '\n', intentional_error=True)
    t1.test('Heslo prilis dlouhe #2: 10000', ['1', '1'], 'A' * 10000 + '\n', intentional_error=True)
    t1.test('Zadna hesla', ['1', '1', '--stats'], '\n', 'Statistika:\nRuznych znaku: 0\nMinimalni delka: 0\nPrumerna delka: 0.0\n')
    t1.test('Velky pocet hesel: 10000', ['1', '1', '--stats'], 'A\n' * 10000, 'Statistika:\nRuznych znaku: 1\nMinimalni delka: 1\nPrumerna delka: 1.0\n')

    if args.bonus:
        # Bonusove reseni
        t2.test('BONUS | Bez argumentu', [], HESLA_TXT, 'Password\nHeslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n')
        t2.test('BONUS | Validni argumenty #1', ['-l', '2', '-p', '4', '--stats'], HESLA_TXT, 'IZP2021:fit@vut.cz\nStatistika:\nRuznych znaku: 36\nMinimalni delka: 8\nPrumerna delka: 14.4\n')
        t2.test('BONUS | Validni argumenty #2', ['--stats', '-p', '4', '-l', '2'], HESLA_TXT, 'IZP2021:fit@vut.cz\nStatistika:\nRuznych znaku: 36\nMinimalni delka: 8\nPrumerna delka: 14.4\n')
        t2.test('BONUS | Neexistujici prepinac', ['-l', '2', '-i', '1'], HESLA_TXT, intentional_error=True)
        t2.test('BONUS | Neexistujici hodnota po prepinaci', ['-p'], HESLA_TXT, intentional_error=True)
        t2.test('BONUS | Prepinac za prepinacem', ['-p', '-l'], HESLA_TXT, intentional_error=True)

    if args.crlf:
        # CRLF podpora
        t3.test('CRLF | Test ze zadani ale CRLF #1', ['1', '1'], HESLA_TXT_CRLF, 'Password\nHeslo123\nMojevelmidlouhehesloscislem0\nIZP2021:fit@vut.cz\n')
        t3.test('CRLF | Test ze zadani ale CRLF #2', ['2', '4', '--stats'], HESLA_TXT_CRLF, 'IZP2021:fit@vut.cz\nStatistika:\nRuznych znaku: 36\nMinimalni delka: 8\nPrumerna delka: 14.4\n')
        t3.test('CRLF | Vstup bez noveho radku na konci', ['1', '1', '--stats'], 'Aaaa\r\nAAAa', 'Aaaa\nAAAa\nStatistika:\nRuznych znaku: 2\nMinimalni delka: 4\nPrumerna delka: 4.0\n')

    print('-- STATISTIKA --')
    print('Zakladni reseni:')
    t1.print_stats()

    if args.bonus:
        print('Bonusove reseni')
        t2.print_stats()

    if args.crlf:
        print('CRLF podpora')
        t3.print_stats()

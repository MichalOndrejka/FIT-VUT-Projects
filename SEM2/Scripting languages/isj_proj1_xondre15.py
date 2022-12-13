#!/usr/bin/env python3

import re

# ukol za 3 body
def camel_to_snake_case(name):
    """Transfer camelCaseNames to snake_case_names.

    >>> camel_to_snake_case('camelCaseNameAllowed')
    'camel_case_name_allowed'
    >>> camel_to_snake_case('longVATNumber')
    'long_vat_number'
    """

    inbetween = re.compile(r'''
                            (
                            (?<=[a-z])# between a lowercase and
                            (?=[A-Z])# an uppercase letter
                            |           # or
                            (?=[A-Z]# in front of an uppercase
                            [a-z])# followed by a lowercase
                            )
                            ''', re.VERBOSE)
    return inbetween.sub(r'_', name).lower()


# ukol za 2 body
def not_both_titles(names_string):
    """Returns a list of names not preceded by [Pp]rof./[Dd]oc. and 
       followed by ', Ph.D.'

    >>> not_both_titles('doc. Josef Tyl, Rudolf Srp, Ph.D., Pavel Vlk, doc. RNDr. Petr Berka, Ph.D., Jan Hora')
    ['doc. Josef Tyl', 'Rudolf Srp, Ph.D.', 'Pavel Vlk', 'Jan Hora']
    """

    # a name that is either preceded by [Pp]rof./[Dd]oc. and followed by Ph.D.
    # or other name with potential titles
    pat = re.compile(r'''
                        (?:[Pp]rof\.|[Dd]oc\.)[^,]*,\sPh\.D\.         # it is either
                        |                                             # or  
                        [\s]*([^,]+(?:,\s[A-Za-z]+\.[A-Za-z]+\.)?),?\s*              # it is
                      ''', re.X)
    return [g1 for g1 in pat.findall(names_string) if g1]


if __name__ == "__main__":
    import doctest
    doctest.testmod()

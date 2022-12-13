; Autor reseni: Michal Ondrejka xondre15

; Projekt 2 - INP 2022
; Vernamova sifra na architekture MIPS64

; DATA SEGMENT
                .data
login:          .asciiz "xondre15"  ; sem doplnte vas login 

cipher:         .space  17  ; misto pro zapis sifrovaneho loginu

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize "funkce" print_string)

; CODE SEGMENT
                .text

        ; ZDE NAHRADTE KOD VASIM RESENIM <=
main:
                ;xondre15-r28-r13-r9-r6-r4-r0
                daddi r28, r0, 0     ;set r13 to 0 (index of login)
while:
                lb r13, login(r28)  ;load first char from login
                slti r9, r13, 97    ;if r13 < 97 => r9 = 1, else r9 = 0
                bne r9, r0, while_end  ;if r9 == 0 go to while_end

                daddi r9, r0, 1     ;put mask 0x0000 0001 to r9
                and r9, r28, r9     ;if current index and mask is 1 it is an odd index, else even index
                beq r9, r0, else_add   ;if r9 == 0 => it is an even index => goto else_add => else go to if_subtract
                b if_substract      ;else go to if_subtract

if_substract:
                daddi r13, r13, -14 ;subtract ascii value of n from char -14
                b end_if            ;skip to end_if so else_add will not run

else_add:
                daddi r13, r13, 15  ;add ascii value of o to char 15
                b end_if

end_if:

                slti r9, r13, 97    ;if r13 < 97 => r9 = 1, else r9 = 0
                bne r9, r0, must_add   ;if r9 == 0 go to must_add
                slti r9, r13, 123   ;if r13 < 123 => r9 = 1, else r9 = 0
                bne r9, r0, must_end   ;if r9 == 0 go to must_add
                b must_sub          ;else go to must_sub

must_add:
                daddi r13, r13, 26  ;add 26 to char
                b must_end          ;go to must_end

must_sub:
                daddi r13, r13, -26 ;sub 26 from char
                b must_end          ;go to must_end

must_end:
                sb r13, cipher(r28) ;save encoded char to cipher at index r28

                daddi r28, r28, 1   ;increase index r28
                b while	            ;jump back to top of while:

while_end:
                daddi r4, r0, cipher ;vozrovy vypis: adresa login: do r4
                jal print_string    ;vypis pomoci print_string - viz nize

                syscall 0           ;halt

print_string:   ; adresa retezce se ocekava v r4
        sw      r4, params_sys5(r0)
        daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
        syscall 5   ; systemova procedura - vypis retezce na terminal
        jr      r31 ; return - r31 je urcen na return address

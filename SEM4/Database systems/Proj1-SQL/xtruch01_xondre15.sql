-- DROP --
DROP TABLE POBYT_T;
DROP TABLE IZBA_T;
DROP TABLE APARTMAN_T;
DROP TABLE ZAKAZNIK_T;
DROP TABLE SPRAVCA_T;
-- DROP SEQUENCE zakaznik_id;
DROP MATERIALIZED VIEW ZAKAZNIK_POBYT_MV;


-- CREATE --

-- SPRAVCA_T dedi od ZAKAZNIK_T
-- Ak ZAKAZNIK_T obsahuje Spravca_ID, tak je to spravca
-- Ak ma v Spravca_ID NULL, tak je to obycajny zakaznik
-- To iste plati pre APARTMAN_T a IZBA_T

CREATE TABLE SPRAVCA_T (
    SpravcaID int GENERATED AS IDENTITY NOT NULL PRIMARY KEY
);

CREATE TABLE ZAKAZNIK_T (
    ZakaznikID int GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    Meno varchar(255) NOT NULL,
    Priezvisko varchar(255) NOT NULL,
    Adresa varchar(255) NOT NULL,
    Mail varchar(255) NOT NULL,
        CHECK(REGEXP_LIKE(
			Mail, '^[a-z]+[a-z0-9\.]*@[a-z0-9\.-]+\.[a-z]{2,}$', 'i'
		)),
    
    Spravca_ID INT DEFAULT NULL,
    CONSTRAINT ZakaznikSpravca_ID_FK
        FOREIGN KEY(Spravca_ID) REFERENCES SPRAVCA_T (SpravcaID)
        ON DELETE SET NULL
);

CREATE TABLE APARTMAN_T(
    CisloApartmanu int NOT NULL PRIMARY KEY,
    PremioveVybavenie varchar(255)
);

CREATE TABLE IZBA_T (
    CisloIzby int NOT NULL PRIMARY KEY,
    Poschodie int NOT NULL,
    Blok varchar(255) NOT NULL,
    Dostupnost varchar(255) NOT NULL,
        CHECK(Dostupnost IN ('dostupna', 'nedostupna')),
    Vybavenie varchar(255),
    Apartman_ID INT DEFAULT NULL,
    
    CONSTRAINT IzbaApartman_ID_FK
        FOREIGN KEY(Apartman_ID) REFERENCES APARTMAN_T (CisloApartmanu)
        ON DELETE SET NULL
);

CREATE TABLE POBYT_T (
    PobytID int GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    ZaciatokPobytu date NOT NULL,
    KoniecPobytu date NOT NULL,
    Poziadavky varchar(255),
    SposobPlatby varchar(255) NOT NULL,
        CHECK(SposobPlatby IN ('karta', 'hotovost', 'bankovy prevod', 'kredit')),
    
    Zakaznik_ID int NOT NULL,
    Spravca_ID int DEFAULT NULL,
    Izba_ID INT DEFAULT NULL,
    CONSTRAINT PobytZakaznik_ID_FK
        FOREIGN KEY (Zakaznik_ID) REFERENCES ZAKAZNIK_T (ZakaznikID)
        ON DELETE CASCADE,
    CONSTRAINT PobytSpravca_ID_FK
        FOREIGN KEY (Spravca_ID) REFERENCES SPRAVCA_T (SpravcaID)
        ON DELETE CASCADE,
    CONSTRAINT IzbaPobyt_ID_FK
        FOREIGN KEY(Izba_ID) REFERENCES IZBA_T (CisloIzby)
        ON DELETE CASCADE
);

-- TRIGGERS --

--CREATE SEQUENCE zakaznik_id;
--CREATE OR REPLACE TRIGGER zakaznik_id
--	BEFORE INSERT ON ZAKAZNIK_T
--	FOR EACH ROW
--BEGIN
--	IF :NEW.ZakaznikID IS NULL THEN
--		:NEW.ZakaznikID := zakaznik_id.NEXTVAL;
--	END IF;
--END;

-- INSERTS --

-- (Spravcovia):
INSERT INTO SPRAVCA_T VALUES(DEFAULT);
INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Pista', 'Lakatos', 'Brno', 'pista.lakatos@gmail.com', 1);

INSERT INTO SPRAVCA_T VALUES(DEFAULT);
INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Jozef', 'Canecky', 'Praha', 'jozef.canecky@gmail.com', 2);

-- (Zakaznici):

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko,Adresa, Mail, Spravca_ID)
VALUES('Dominik', 'Truchly', 'Zilina', 'dodo.truchly.11@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko,Adresa, Mail, Spravca_ID)
VALUES('Michal', 'Ondrejka', 'Zilina', 'michal.ondrejka.sk@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Peter', 'Tichy', 'Bratislava', 'peter.tichy@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Martin', 'Toth', 'Breclav', 'martin.toth@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Andrej', 'Hudecky', 'Olomouc', 'andrej.hudecky@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Igor', 'Hrlecko', 'Bratislava', 'igor.hrlecko@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Martin', 'Klabouch', 'Praha', 'martin.klabouch@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Tomas', 'Ondrejka', 'Seoul', 'tomas.ondrejka@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Anna', 'Slavikova', 'Trnava', 'anna.slavikova@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Klara', 'Kovackova', 'Olomouc', 'klara.kovacova@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Katarina', 'Zavadska', 'Bohumin', 'katarina.zavadska@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Petra', 'Spkojona', 'Ostrava', 'petra.spokojna@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Alexandra', 'Nagyova', 'Pardubice', 'alexandra.nagyova@gmail.com', NULL);

INSERT INTO ZAKAZNIK_T (Meno, Priezvisko, Adresa, Mail, Spravca_ID)
VALUES('Timea', 'Molnarova', 'Trencin', 'timea.molnarova@gmail.com', NULL);

-- (Apartmany)

INSERT INTO APARTMAN_T (CisloApartmanu, PremioveVybavenie) 
VALUES(1601, 'sauna, zehlicka');
INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(1601, 6, 'B01', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', 1601);

INSERT INTO APARTMAN_T (CisloApartmanu, PremioveVybavenie) 
VALUES(1602, 'sauna, kada');
INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(1602, 6, 'B01', 'nedostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', 1602);

-- (Izby):

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(2101, 1,'B02', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(2102, 1,'B02', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(2201, 2,'B02', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(2301, 3,'B02', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(3101, 1,'B03', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(3102, 1,'B03', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(3201, 2,'B03', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(4101, 1,'B04', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

INSERT INTO IZBA_T (CisloIzby, Poschodie, Blok, Dostupnost, Vybavenie,  Apartman_ID)
VALUES(4201, 2,'B04', 'dostupna', 'mikrovlnna rura, chladnicka, rychlovarna kanvica', NULL);

-- (Pobyty):

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2023-03-10', 'yyyy/mm/dd'), TO_DATE('2023-03-15', 'yyyy/mm/dd'), 'Ruzu na postel', 'karta', 1, 1, 1601);

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2022-02-27', 'yyyy/mm/dd'), TO_DATE('2022-03-02', 'yyyy/mm/dd'), NULL, 'kredit', 3, 2, 2101);

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2021-09-12', 'yyyy/mm/dd'), TO_DATE('2021-09-20', 'yyyy/mm/dd'), NULL, 'bankovy prevod', 5, 1, 2201);

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2022-06-21', 'yyyy/mm/dd'), TO_DATE('2022-06-23', 'yyyy/mm/dd'), 'Lad v mraznicke', 'hotovost', 7, 2, 3101);

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2023-05-02', 'yyyy/mm/dd'), TO_DATE('2023-05-06', 'yyyy/mm/dd'), 'Ruzu na postel', 'karta', 9, NULL, 3201);

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2023-07-25', 'yyyy/mm/dd'), TO_DATE('2023-08-05', 'yyyy/mm/dd'), NULL, 'karta', 11, NULL, 4101);

INSERT INTO POBYT_T (ZaciatokPobytu, KoniecPobytu, Poziadavky, SposobPlatby,  Zakaznik_ID, Spravca_ID, Izba_ID)
VALUES(TO_DATE('2023-12-30', 'yyyy/mm/dd'), TO_DATE('2024-01-02', 'yyyy/mm/dd'), 'Flasa sampanskeho v chladnicke', 'hotovost', 13, NULL, 4201);

-- SELECTS --

--SELECT * FROM SPRAVCA_T;
--SELECT * FROM ZAKAZNIK_T;
--SELECT * FROM POBYT_T;
--SELECT * FROM IZBA_T;
--SELECT * FROM APARTMAN_T;


-- 1. Vyberie meno a priezvisko ludi ktori stravili svoj pobyt v Apartmane
SELECT ZAKAZNIK_T.MENO, ZAKAZNIK_T.PRIEZVISKO FROM POBYT_T INNER JOIN ZAKAZNIK_T ON POBYT_T.ZAKAZNIK_ID = ZAKAZNIK_T.ZAKAZNIKID INNER JOIN IZBA_T ON POBYT_T.IZBA_ID = IZBA_T.CISLOIZBY
WHERE IZBA_T.APARTMAN_ID is not null;

-- 2. Vyberie zakaznikov ktori zacali a aj skoncili svoj pobyt v roku 2022
SELECT * FROM ZAKAZNIK_T INNER JOIN POBYT_T ON POBYT_T.ZAKAZNIK_ID = ZAKAZNIK_T.ZAKAZNIKID
WHERE POBYT_T.KONIECPOBYTU <= '31-DEC-22' and POBYT_T.ZACIATOKPOBYTU >= '01-JAN-22';

-- 3. Vypise cislo apartmanu, jeho zakladne a aj premiove vybavenie
SELECT CISLOAPARTMANU, IZBA_T.VYBAVENIE, PREMIOVEVYBAVENIE FROM APARTMAN_T INNER JOIN IZBA_T ON APARTMAN_T.CisloApartmanu = IZBA_T.APARTMAN_ID;

-- 4. Vypise kolkokrat sa pouzil aky sposob platby za pobyt
SELECT SposobPlatby, COUNT(*) AS Pocet
FROM POBYT_T
GROUP BY SposobPlatby;

-- 5. Vypise kolko poschodi sa nachadza na jednotlivych blokoch
SELECT BLOK, MAX(POSCHODIE) as Pocet_Poschodi
FROM IZBA_T
GROUP BY BLOK;

-- TRIGGERS --

--CREATE SEQUENCE zakaznik_id;
--CREATE OR REPLACE TRIGGER zakaznik_id
--	BEFORE INSERT ON ZAKAZNIK_T
--	FOR EACH ROW
--BEGIN
--	IF :NEW.ZakaznikID IS NULL THEN
--		:NEW.ZakaznikID := zakaznik_id.NEXTVAL;
--	END IF;
--END;

-- EXPLAIN PLAN --

-- Ktorí uživatelia strávili svoj pobyt v Apartmane
EXPLAIN PLAN FOR SELECT ZAKAZNIK_T.MENO, ZAKAZNIK_T.PRIEZVISKO FROM POBYT_T INNER JOIN ZAKAZNIK_T ON POBYT_T.ZAKAZNIK_ID = ZAKAZNIK_T.ZAKAZNIKID INNER JOIN IZBA_T ON POBYT_T.IZBA_ID = IZBA_T.CISLOIZBY
WHERE IZBA_T.APARTMAN_ID is not null;
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

-- Ktorí zákazníci zacali a aj skoncili svoj pobyt v roku 2022
EXPLAIN PLAN FOR SELECT * FROM ZAKAZNIK_T INNER JOIN POBYT_T ON POBYT_T.ZAKAZNIK_ID = ZAKAZNIK_T.ZAKAZNIKID
WHERE POBYT_T.KONIECPOBYTU <= '31-DEC-22' and POBYT_T.ZACIATOKPOBYTU >= '01-JAN-22';
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

-- MATERALIZED VIEW --

-- Materializovaný poh?ad na všetkých uživate?ov a v ktorej izbe sú ubytovaní.
CREATE MATERIALIZED VIEW ZAKAZNIK_POBYT_MV AS
SELECT ZAKAZNIK_T.ZakaznikID, ZAKAZNIK_T.Meno, ZAKAZNIK_T.Priezvisko, POBYT_T.Izba_ID
FROM ZAKAZNIK_T
LEFT JOIN POBYT_T ON POBYT_T.Zakaznik_ID = ZAKAZNIK_T.ZakaznikID;

-- Výpis Materializovaný poh?adu
SELECT * FROM ZAKAZNIK_POBYT_MV;

-- PRIVILEGES --

-- xtruch01 práva
GRANT ALL ON SPRAVCA_T TO xtruch01;
GRANT ALL ON ZAKAZNIK_T TO xtruch01;
GRANT ALL ON APARTMAN_T TO xtruch01;
GRANT ALL ON IZBA_T TO xtruch01;
GRANT ALL ON POBYT_T TO xtruch01;
GRANT ALL ON ZAKAZNIK_POBYT_MV TO xtruch01;

-- xondre15 práva
GRANT SELECT, INSERT ON SPRAVCA_T TO xondre15;
GRANT SELECT, INSERT ON ZAKAZNIK_T TO xondre15;
GRANT SELECT, INSERT ON APARTMAN_T TO xondre15;
GRANT SELECT, INSERT ON IZBA_T TO xondre15;
GRANT SELECT, INSERT ON POBYT_T TO xondre15;
GRANT SELECT, INSERT ON ZAKAZNIK_POBYT_MV TO xondre15;
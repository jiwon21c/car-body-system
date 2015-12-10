# Áttekintés #

Hasznos információk automatikus beillesztése a forráskódba.


# Részletek #

A leggyakrabban használt kulcsszavakat az svn commitoláskor automatikusan képes kitölteni. Ezeke lehetnek az aktuális commitot végző szerző, a commitolás dátuma és a revision száma. Ehhez minden forrásfájlt kommentezni kell az alábbi szerint:
```
// Utolsó módosítást végezte: $Author$
// Utolsó módosítás dátuma:   $LastChangedDate$
// Revision:                  $Rev$
```
Ezek után ki kell választani azokat a forrásfájlokat amiken szeretnénk alkalmazni. Jobb klikk -> tortoise svn -> properties. Ezután a New gombra kell kattintani. A property name-nél az svn:keywords-t kell választani. A property value-ba pedig az alábbiakat kell beírni **enterrel** elválasztva egymástól:

Author
LastChangedDate
Rev

Ezt elvégezve a commitolásnál az svn automatikusan kicseréli $$ közé írt szavakat a megfelelő adatokra.
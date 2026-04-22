<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="hr_HR">
<context>
    <name>CullendulaFileSystemHandler</name>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="53"/>
        <source>Could not move &apos;%1&apos; to &apos;%2&apos;: %3</source>
        <extracomment>Error message after moving a file into a named subdirectory such as &quot;output&quot;, &quot;trash&quot;, &quot;undo&quot;, or &quot;redo&quot; failed.</extracomment>
        <translation>Nije moguće premjestiti &apos;%1&apos; u &apos;%2&apos;: %3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="58"/>
        <source>Could not prepare &apos;%1&apos; directory at &apos;%2&apos;: %3</source>
        <extracomment>Error message after preparing an application-managed subdirectory such as &quot;output&quot; or &quot;trash&quot; failed.</extracomment>
        <translation>Nije moguće pripremiti direktorij &apos;%1&apos; na &apos;%2&apos;: %3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="206"/>
        <source>showing %1 of %2</source>
        <extracomment>Status bar message showing the current 1-based image position and the total number of loaded images.</extracomment>
        <translation>prikazuje %1 od %2</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="231"/>
        <source>No undo step is currently available.</source>
        <extracomment>Error shown when the user requests Undo but the undo history is empty.</extracomment>
        <translation>Trenutno nije dostupan nijedan korak poništavanja.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="248"/>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="285"/>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="474"/>
        <source>the filesystem rename operation failed</source>
        <extracomment>Low-level filesystem failure detail inserted into a larger user-visible move error message.</extracomment>
        <translation>operacija preimenovanja datotečnog sustava nije uspjela</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="262"/>
        <source>No redo step is currently available.</source>
        <extracomment>Error shown when the user requests Redo but the redo history is empty.</extracomment>
        <translation>Trenutno nije dostupan nijedan korak ponavljanja.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="346"/>
        <source>The path &apos;%1&apos; could not be resolved to an existing directory.</source>
        <extracomment>Error after a dropped path or selected path does not resolve to an existing directory on disk.</extracomment>
        <translation>Put &apos;%1&apos; ne može se razriješiti na postojeći direktorij.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="415"/>
        <source>the path is already occupied by a non-directory filesystem entry</source>
        <extracomment>Failure detail for a managed output directory path that already exists as a regular file or another non-directory entry.</extracomment>
        <translation>staza je već zauzeta unosom datotečnog sustava koji nije direktorij</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="427"/>
        <source>creating the directory failed</source>
        <extracomment>Failure detail for a managed output directory that Qt could not create on disk.</extracomment>
        <translation>stvaranje imenika nije uspjelo</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="434"/>
        <source>the directory is still missing after creation</source>
        <extracomment>Failure detail for a managed output directory that still does not exist after a reported creation attempt.</extracomment>
        <translation>imenik još uvijek nedostaje nakon izrade</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="452"/>
        <source>No current image is available to move.</source>
        <extracomment>Error shown when the user tries to save or trash an image but no current image is loaded.</extracomment>
        <translation>Trenutna slika nije dostupna za premještanje.</translation>
    </message>
</context>
<context>
    <name>CullendulaMainWindow</name>
    <message>
        <source>Cullendula - your smol helper to pick the best shots</source>
        <translation type="vanished">Cullendula - vaš smol pomoćnik u odabiru najboljih kadrova</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;This label will show the current photo (image-file) in case one is loaded.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Please drag and drop a directory or file on this area.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Ova oznaka će prikazati trenutnu fotografiju (datoteku-slike) u slučaju da je ona učitana.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Molimo vas da povučete i ispustite direktorij ili datoteku na ovo područje.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>drag&amp;drop folder or file to start :)
(loads the enabled extensions from Main -&gt; Extensions, for example *.jpg, *.jpeg, *.png, *.webp)</source>
        <translation type="vanished">povucite i ispustite mapu ili datoteku za početak :) 
(učitava omogućena proširenja iz Main -&gt; Extensions, na primjer *.jpg, *.jpeg, *.png, *.webp)</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the left neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;left&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Prebaci se na lijevog susjeda (na temelju popisa datoteka trenutnog direktorija).&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;lijevu&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>←</source>
        <translation type="vanished">←</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="vanished">Lijevo</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified output-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;up&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Premjesti trenutnu sliku u navedeni izlazni poddirektorij. Automatski će se prebaciti na fotografiju koja je sljedeća&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;gore&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>save ↑</source>
        <translation type="vanished">spremiti ↑</translation>
    </message>
    <message>
        <source>Up</source>
        <translation type="vanished">Gore</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified trash-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;down&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Premjesti trenutnu sliku u navedeni poddirektorij smeća. Automatski će se prebaciti na fotografiju koja je sljedeća&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;dolje&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>trash ↓</source>
        <translation type="vanished">smeće ↓</translation>
    </message>
    <message>
        <source>Down</source>
        <translation type="vanished">dolje</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the right neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;right&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Prebaci se na desni susjed (na temelju popisa datoteka trenutnog direktorija).&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;desno&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>→ </source>
        <translation type="vanished">→</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="vanished">Pravo</translation>
    </message>
    <message>
        <source>This is the status bar. It will display any kind of information like current position, failure messages, ..</source>
        <translation type="vanished">Ovo je statusna traka. Prikazat će bilo koju vrstu informacija poput trenutne pozicije, poruka o neuspjehu, ..</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="202"/>
        <source>system is up and running :)</source>
        <extracomment>Status bar message shown once after the main window is fully initialized.</extracomment>
        <translation>sustav radi :)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="231"/>
        <source>drop current load and let&apos;s see what you dragged?</source>
        <extracomment>Status bar prompt during drag-and-drop after the payload was recognized as file-system URLs.</extracomment>
        <translation>ispusti trenutno opterećenje i da vidimo što si dovukao?</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="271"/>
        <source>The load was not usable! :(</source>
        <extracomment>Status bar error after a drop payload was rejected because it did not contain usable local file URLs.</extracomment>
        <translation>Teret nije bio upotrebljiv! :(</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="326"/>
        <source>Could not save the current file.</source>
        <extracomment>Fallback status bar error after moving the current image to the &quot;output&quot; folder failed without a more specific message.</extracomment>
        <translation>Nije moguće spremiti trenutnu datoteku.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="345"/>
        <source>Could not trash the current file.</source>
        <extracomment>Fallback status bar error after moving the current image to the &quot;trash&quot; folder failed without a more specific message.</extracomment>
        <translation>Nije moguće baciti trenutnu datoteku u smeće.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="417"/>
        <source>no more valid images found: work maybe finished? :)
drag&amp;drop the next folder or files if you want!</source>
        <extracomment>Center label placeholder when no further images match the current extension filter in the active directory.</extracomment>
        <translation>više nije pronađena važeća slika: rad je možda završen? :) 
povucite i ispustite sljedeću mapu ili datoteke ako želite!</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="423"/>
        <source>no more files</source>
        <extracomment>Status bar message when the current directory no longer contains any matching images to show.</extracomment>
        <translation>nema više datoteka</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="430"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="432"/>
        <source>could not load the current image preview</source>
        <extracomment>Error shown both in the center label and the status bar when Qt cannot render the current image preview.</extracomment>
        <translation>nije mogao učitati trenutni pregled slike</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="478"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="640"/>
        <source>Enable loading of *.%1 files when opening the next directory</source>
        <extracomment>Tooltip for a checkable menu entry that enables a filename extension such as jpg, png, or webp for future directory scans.</extracomment>
        <translation>Omogući učitavanje *.%1 datoteka prilikom otvaranja sljedećeg direktorija</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="484"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="646"/>
        <source>Light</source>
        <extracomment>Menu label for the light visual theme.</extracomment>
        <translation>Svjetlo</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="488"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="648"/>
        <source>Use the light application theme</source>
        <extracomment>Tooltip for switching the whole application to the light theme.</extracomment>
        <translation>Koristite svijetlu temu aplikacije</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="492"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="653"/>
        <source>Dark</source>
        <extracomment>Menu label for the dark visual theme.</extracomment>
        <translation>tamno</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="496"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="655"/>
        <source>Use the high-contrast dark application theme</source>
        <extracomment>Tooltip for switching the whole application to the dark theme.</extracomment>
        <translation>Koristite tamnu temu aplikacije visokog kontrasta</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="503"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="660"/>
        <source>English</source>
        <extracomment>Language menu entry naming the English user-interface language.</extracomment>
        <translation>engleski</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="510"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="667"/>
        <source>Deutsch</source>
        <extracomment>Language menu entry naming the German user-interface language in German.</extracomment>
        <translation>Deutsch</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="517"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="674"/>
        <source>Hrvatski</source>
        <extracomment>Language menu entry naming the Croatian user-interface language in Croatian.</extracomment>
        <translation>Hrvatski</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="524"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="681"/>
        <source>中文</source>
        <extracomment>Language menu entry naming the Chinese user-interface language in Chinese.</extracomment>
        <translation>中文</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="532"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="693"/>
        <source>Undo</source>
        <extracomment>Edit menu action label that reverses the most recent file move.</extracomment>
        <translation>Poništi</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="534"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="695"/>
        <source>Revert the last file-move-operation</source>
        <extracomment>Tooltip for the Undo action that moves the previously moved image back to its original location.</extracomment>
        <translation>Vraćanje zadnje operacije premještanja datoteke</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="543"/>
        <source>Could not undo the last file move.</source>
        <extracomment>Fallback status bar error after an undo request failed without a more specific message.</extracomment>
        <translation>Nije moguće poništiti zadnje premještanje datoteke.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="549"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="700"/>
        <source>Redo</source>
        <extracomment>Edit menu action label that reapplies the most recently undone file move.</extracomment>
        <translation>Ponovi</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="551"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="702"/>
        <source>Redo the last file-move-operation (means: undo undo)</source>
        <extracomment>Tooltip for the Redo action. &quot;undo undo&quot; here means reapplied after an Undo.</extracomment>
        <translation>Ponovi posljednju operaciju premještanja datoteke (znači: poništi poništavanje)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="560"/>
        <source>Could not redo the last file move.</source>
        <extracomment>Fallback status bar error after a redo request failed without a more specific message.</extracomment>
        <translation>Nije moguće ponoviti zadnje premještanje datoteke.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="567"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="712"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="803"/>
        <source>About Cullendula</source>
        <extracomment>Help menu action label that opens the application&apos;s About dialog.
----------
Title of the application&apos;s About dialog.</extracomment>
        <translation>O Cullenduli</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="569"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="714"/>
        <source>Show the application&apos;s About box</source>
        <extracomment>Tooltip for opening the application&apos;s About dialog.</extracomment>
        <translation>Prikaži okvir O aplikaciji</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="574"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="719"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="821"/>
        <source>About Qt</source>
        <extracomment>Help menu action label that opens Qt&apos;s built-in About dialog.
----------
Title of the Qt runtime information dialog. Rich-text body of the Qt runtime information dialog. %1 is the Qt version, %2 is the Qt installation prefix path.</extracomment>
        <translation>O Qt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="576"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="721"/>
        <source>Show the Qt library&apos;s About box</source>
        <extracomment>Tooltip for opening Qt&apos;s built-in About dialog.</extracomment>
        <translation>Prikaži okvir O Qt biblioteci</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="614"/>
        <source>Cullendula</source>
        <extracomment>Main window title; the version suffix is appended separately in code.</extracomment>
        <translation>Cullendula</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="618"/>
        <source>Main</source>
        <extracomment>Top-level menu containing application settings such as extensions, style, and language.</extracomment>
        <translation>Glavni</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="623"/>
        <source>Extensions</source>
        <extracomment>Submenu listing the enabled image filename extensions for directory scanning.</extracomment>
        <translation>Ekstenzije</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="628"/>
        <source>Style</source>
        <extracomment>Submenu for switching between visual themes.</extracomment>
        <translation>Stil</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="633"/>
        <source>Language</source>
        <extracomment>Submenu for switching the user-interface language.</extracomment>
        <translation>Jezik</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="662"/>
        <source>Use the default English source texts</source>
        <extracomment>Tooltip for switching back to the original English source texts.</extracomment>
        <translation>Koristite zadani engleski izvorni tekst</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="669"/>
        <source>Load the German user-interface translation</source>
        <extracomment>Tooltip for loading the German translation file.</extracomment>
        <translation>Učitajte prijevod njemačkog korisničkog sučelja</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="676"/>
        <source>Load the Croatian user-interface translation</source>
        <extracomment>Tooltip for loading the Croatian translation file.</extracomment>
        <translation>Učitajte prijevod hrvatskog korisničkog sučelja</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="683"/>
        <source>Load the Chinese user-interface translation</source>
        <extracomment>Tooltip for loading the Simplified Chinese translation file.</extracomment>
        <translation>Učitajte prijevod kineskog korisničkog sučelja</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="688"/>
        <source>Edit</source>
        <extracomment>Top-level menu for undo and redo actions.</extracomment>
        <translation>Uredi</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="707"/>
        <source>Help</source>
        <extracomment>Top-level menu for About dialogs and other help-related actions.</extracomment>
        <translation>Pomoć</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="743"/>
        <source>Could not load the selected language.</source>
        <extracomment>Status bar error when loading the requested translation catalog failed.</extracomment>
        <translation>Nije moguće učitati odabrani jezik.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="801"/>
        <source>Invoked Help|About</source>
        <extracomment>Status bar trace shown when the user opens the application&apos;s About dialog from the Help menu.</extracomment>
        <translation>Pozvana pomoć|O</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="805"/>
        <source>Helper program to sort out (&quot;cull&quot;) a collection of pictures in a directory after a nice photo-walk or event.&lt;br&gt;Should work cross-platform.&lt;br&gt;&lt;br&gt;Developed by &lt;a href=&apos;mail@marcelpetrick.it&apos;&gt;mail@marcelpetrick.it&lt;/a&gt;&lt;br&gt;Source code can be found inside the repository at &lt;a href=&apos;https://github.com/marcelpetrick/Cullendula/&apos;&gt;https://github.com/marcelpetrick/Cullendula&lt;/a&gt;&lt;br&gt;Feel free to use and share: GPL v3 :3</source>
        <extracomment>Rich-text body of the application&apos;s About dialog.</extracomment>
        <translation>Pomoćni program za sortiranje (&quot;oduzimanje&quot;) kolekcije slika u direktoriju nakon lijepe foto-šetnje ili događaja.&lt;br&gt;Trebao bi raditi na više platformi.&lt;br&gt;&lt;br&gt;Razvio &lt;a href=&apos;mail@marcelpetrick.it&apos;&gt;mail@marcelpetrick.it&lt;/a&gt;&lt;br&gt;Izvorni kod može se pronaći unutar repozitorija na &lt;a href=&apos;https://github.com/marcelpetrick/Cullendula/&apos;&gt;https://github.com/marcelpetrick/Cullendula&lt;/a&gt;&lt;br&gt;Slobodno koristite i dijelite: GPL v3 :3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="818"/>
        <source>Invoked Help|About Qt</source>
        <extracomment>Status bar trace shown when the user opens Qt&apos;s About dialog from the Help menu.</extracomment>
        <translation>Pozvana pomoć|O Qt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="821"/>
        <source>This application currently runs with Qt %1.&lt;br&gt;Qt installation prefix: %2&lt;br&gt;&lt;br&gt;Qt is a cross-platform application framework for building desktop and embedded applications.</source>
        <translation>Ova aplikacija trenutno radi s Qt-om %1.&lt;br&gt;Instalacijski prefiks Qt-a: %2&lt;br&gt;&lt;br&gt;Qt je međuplatformski aplikacijski okvir za izradu desktop i ugrađenih aplikacija.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="23"/>
        <source>Cullendula - your smol helper to pick the best shots</source>
        <comment>Main window title shown before the runtime version suffix is applied from C++ code.</comment>
        <translation>Cullendula - vaš smol pomoćnik u odabiru najboljih kadrova</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="55"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;This label will show the current photo (image-file) in case one is loaded.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Please drag and drop a directory or file on this area.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the large central preview area. It accepts a dropped image file or a directory containing images.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Ova oznaka će prikazati trenutnu fotografiju (datoteku-slike) u slučaju da je ona učitana.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Molimo vas da povučete i ispustite direktorij ili datoteku na ovo područje.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="64"/>
        <source>drag&amp;drop folder or file to start :)
(loads the enabled extensions from Main -&gt; Extensions, for example *.jpg, *.jpeg, *.png, *.webp)</source>
        <comment>Placeholder text in the central preview area before any image is loaded.</comment>
        <translation>povucite i ispustite mapu ili datoteku za početak :) 
(učitava omogućena proširenja iz Main -&gt; Extensions, na primjer *.jpg, *.jpeg, *.png, *.webp)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="131"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the left neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;left&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the left navigation button; it shows the previous image in the current directory order.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Prebaci se na lijevog susjeda (na temelju popisa datoteka trenutnog direktorija).&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;lijevu&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="134"/>
        <source>←</source>
        <comment>Label of the previous-image navigation button. Keep the arrow symbol if possible.</comment>
        <translation>←</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="137"/>
        <source>Left</source>
        <comment>Keyboard shortcut for the previous-image navigation action; use the standard Left Arrow key sequence.</comment>
        <translation>Lijevo</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="164"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified output-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;up&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the Save button. This moves the current image into the app-managed &apos;output&apos; subdirectory, it does not write image edits.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Premjesti trenutnu sliku u navedeni izlazni poddirektorij. Automatski će se prebaciti na fotografiju koja je sljedeća&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;gore&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="167"/>
        <source>save ↑</source>
        <comment>Label of the button that keeps the current image by moving it into the &apos;output&apos; subdirectory.</comment>
        <translation>spremiti ↑</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="170"/>
        <source>Up</source>
        <comment>Keyboard shortcut for the Save action; use the standard Up Arrow key sequence.</comment>
        <translation>Gore</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="197"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified trash-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;down&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the Trash button. This moves the current image into the app-managed &apos;trash&apos; subdirectory, it does not delete it permanently.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Premjesti trenutnu sliku u navedeni poddirektorij smeća. Automatski će se prebaciti na fotografiju koja je sljedeća&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;dolje&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="200"/>
        <source>trash ↓</source>
        <comment>Label of the button that rejects the current image by moving it into the &apos;trash&apos; subdirectory.</comment>
        <translation>smeće ↓</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="203"/>
        <source>Down</source>
        <comment>Keyboard shortcut for the Trash action; use the standard Down Arrow key sequence.</comment>
        <translation>dolje</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="232"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the right neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;right&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the right navigation button; it shows the next image in the current directory order.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Prebaci se na desni susjed (na temelju popisa datoteka trenutnog direktorija).&lt;/p&gt;&lt;p&gt;Kao prečac pritisnite &amp;quot;desno&amp;quot; tipka sa strelicom.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="235"/>
        <source>→ </source>
        <comment>Label of the next-image navigation button. Keep the arrow symbol if possible.</comment>
        <translation>→</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="238"/>
        <source>Right</source>
        <comment>Keyboard shortcut for the next-image navigation action; use the standard Right Arrow key sequence.</comment>
        <translation>Pravo</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="277"/>
        <source>This is the status bar. It will display any kind of information like current position, failure messages, ..</source>
        <comment>Tooltip for the status bar at the bottom of the main window.</comment>
        <translation>Ovo je statusna traka. Prikazat će bilo koju vrstu informacija poput trenutne pozicije, poruka o neuspjehu, ..</translation>
    </message>
</context>
</TS>

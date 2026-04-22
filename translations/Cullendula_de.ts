<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE">
<context>
    <name>CullendulaFileSystemHandler</name>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="53"/>
        <source>Could not move &apos;%1&apos; to &apos;%2&apos;: %3</source>
        <extracomment>Error message after moving a file into a named subdirectory such as &quot;output&quot;, &quot;trash&quot;, &quot;undo&quot;, or &quot;redo&quot; failed.</extracomment>
        <translation>„%1“ konnte nicht nach „%2“ verschoben werden: %3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="58"/>
        <source>Could not prepare &apos;%1&apos; directory at &apos;%2&apos;: %3</source>
        <extracomment>Error message after preparing an application-managed subdirectory such as &quot;output&quot; or &quot;trash&quot; failed.</extracomment>
        <translation>Das Verzeichnis „%1“ unter „%2“ konnte nicht vorbereitet werden: %3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="206"/>
        <source>showing %1 of %2</source>
        <extracomment>Status bar message showing the current 1-based image position and the total number of loaded images.</extracomment>
        <translation>%1 von %2 wird angezeigt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="231"/>
        <source>No undo step is currently available.</source>
        <extracomment>Error shown when the user requests Undo but the undo history is empty.</extracomment>
        <translation>Derzeit ist kein Schritt zum Rückgängigmachen verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="248"/>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="285"/>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="474"/>
        <source>the filesystem rename operation failed</source>
        <extracomment>Low-level filesystem failure detail inserted into a larger user-visible move error message.</extracomment>
        <translation>Der Vorgang zum Umbenennen des Dateisystems ist fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="262"/>
        <source>No redo step is currently available.</source>
        <extracomment>Error shown when the user requests Redo but the redo history is empty.</extracomment>
        <translation>Derzeit ist kein Wiederherstellungsschritt verfügbar.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="346"/>
        <source>The path &apos;%1&apos; could not be resolved to an existing directory.</source>
        <extracomment>Error after a dropped path or selected path does not resolve to an existing directory on disk.</extracomment>
        <translation>Der Pfad „%1“ konnte nicht in ein vorhandenes Verzeichnis aufgelöst werden.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="415"/>
        <source>the path is already occupied by a non-directory filesystem entry</source>
        <extracomment>Failure detail for a managed output directory path that already exists as a regular file or another non-directory entry.</extracomment>
        <translation>Der Pfad ist bereits durch einen Nicht-Verzeichnis-Dateisystemeintrag belegt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="427"/>
        <source>creating the directory failed</source>
        <extracomment>Failure detail for a managed output directory that Qt could not create on disk.</extracomment>
        <translation>Das Erstellen des Verzeichnisses ist fehlgeschlagen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="434"/>
        <source>the directory is still missing after creation</source>
        <extracomment>Failure detail for a managed output directory that still does not exist after a reported creation attempt.</extracomment>
        <translation>Das Verzeichnis fehlt nach der Erstellung noch</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="452"/>
        <source>No current image is available to move.</source>
        <extracomment>Error shown when the user tries to save or trash an image but no current image is loaded.</extracomment>
        <translation>Es ist kein aktuelles Bild zum Verschieben verfügbar.</translation>
    </message>
</context>
<context>
    <name>CullendulaMainWindow</name>
    <message>
        <source>Cullendula - your smol helper to pick the best shots</source>
        <translation type="vanished">Cullendula – Ihr kleiner Helfer bei der Auswahl der besten Aufnahmen</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;This label will show the current photo (image-file) in case one is loaded.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Please drag and drop a directory or file on this area.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot;font-size:10pt;&quot;&gt;Diese Beschriftung zeigt das aktuelle Foto (Bilddatei) an, falls eines geladen ist.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot;font-size:10pt;&quot;&gt;Bitte ziehen Sie ein Verzeichnis oder eine Datei per Drag &amp; Drop in diesen Bereich.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>drag&amp;drop folder or file to start :)
(loads the enabled extensions from Main -&gt; Extensions, for example *.jpg, *.jpeg, *.png, *.webp)</source>
        <translation type="vanished">Ziehen Sie den Ordner oder die Datei per Drag&amp;Drop zum Starten :) 
(lädt die aktivierten Erweiterungen von „Haupt“ -&gt; „Erweiterungen“, zum Beispiel *.jpg, *.jpeg, *.png, *.webp)</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the left neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;left&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Wechseln Sie zum linken Nachbarn (basierend auf der Dateiliste des aktuellen Verzeichnisses).&lt;/p&gt;&lt;p&gt;Als Tastenkombination drücken Sie die Taste „links“. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>←</source>
        <translation type="vanished">←</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="vanished">Links</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified output-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;up&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Verschieben Sie das aktuelle Bild in das angegebene Ausgabe-Unterverzeichnis. Wechselt automatisch zum nächsten Foto.&lt;/p&gt;&lt;p&gt;Drücken Sie als Verknüpfung die Taste „Nach oben“. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>save ↑</source>
        <translation type="vanished">speichern ↑</translation>
    </message>
    <message>
        <source>Up</source>
        <translation type="vanished">Hoch</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified trash-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;down&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Verschiebt das aktuelle Bild in das angegebene Trash-Unterverzeichnis. Wechselt automatisch zum nächsten Foto.&lt;/p&gt;&lt;p&gt;Drücken Sie als Verknüpfung die Taste „Nach unten“. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>trash ↓</source>
        <translation type="vanished">Müll ↓</translation>
    </message>
    <message>
        <source>Down</source>
        <translation type="vanished">Runter</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the right neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;right&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Wechseln Sie zum rechten Nachbarn (basierend auf der Dateiliste des aktuellen Verzeichnisses).&lt;/p&gt;&lt;p&gt;Als Tastenkombination drücken Sie die „rechte“ Taste. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>→ </source>
        <translation type="vanished">→</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="vanished">Rechts</translation>
    </message>
    <message>
        <source>This is the status bar. It will display any kind of information like current position, failure messages, ..</source>
        <translation type="vanished">Dies ist die Statusleiste. Es werden alle Arten von Informationen wie die aktuelle Position, Fehlermeldungen usw. angezeigt.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="202"/>
        <source>system is up and running :)</source>
        <extracomment>Status bar message shown once after the main window is fully initialized.</extracomment>
        <translation>System ist betriebsbereit :)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="231"/>
        <source>drop current load and let&apos;s see what you dragged?</source>
        <extracomment>Status bar prompt during drag-and-drop after the payload was recognized as file-system URLs.</extracomment>
        <translation>Lass die aktuelle Last fallen und lass uns sehen, was du gezogen hast?</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="271"/>
        <source>The load was not usable! :(</source>
        <extracomment>Status bar error after a drop payload was rejected because it did not contain usable local file URLs.</extracomment>
        <translation>Die Ladung war nicht nutzbar! :(</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="326"/>
        <source>Could not save the current file.</source>
        <extracomment>Fallback status bar error after moving the current image to the &quot;output&quot; folder failed without a more specific message.</extracomment>
        <translation>Die aktuelle Datei konnte nicht gespeichert werden.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="345"/>
        <source>Could not trash the current file.</source>
        <extracomment>Fallback status bar error after moving the current image to the &quot;trash&quot; folder failed without a more specific message.</extracomment>
        <translation>Die aktuelle Datei konnte nicht in den Papierkorb verschoben werden.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="417"/>
        <source>no more valid images found: work maybe finished? :)
drag&amp;drop the next folder or files if you want!</source>
        <extracomment>Center label placeholder when no further images match the current extension filter in the active directory.</extracomment>
        <translation>Keine gültigen Bilder mehr gefunden: Arbeit vielleicht fertig? :) :) 
Ziehen Sie den nächsten Ordner oder die nächsten Dateien per Drag &amp; Drop, wenn Sie möchten!</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="423"/>
        <source>no more files</source>
        <extracomment>Status bar message when the current directory no longer contains any matching images to show.</extracomment>
        <translation>keine Dateien mehr</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="430"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="432"/>
        <source>could not load the current image preview</source>
        <extracomment>Error shown both in the center label and the status bar when Qt cannot render the current image preview.</extracomment>
        <translation>Die aktuelle Bildvorschau konnte nicht geladen werden</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="478"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="640"/>
        <source>Enable loading of *.%1 files when opening the next directory</source>
        <extracomment>Tooltip for a checkable menu entry that enables a filename extension such as jpg, png, or webp for future directory scans.</extracomment>
        <translation>Aktivieren Sie das Laden von *.%1-Dateien beim Öffnen des nächsten Verzeichnisses</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="484"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="646"/>
        <source>Light</source>
        <extracomment>Menu label for the light visual theme.</extracomment>
        <translation>Licht</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="488"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="648"/>
        <source>Use the light application theme</source>
        <extracomment>Tooltip for switching the whole application to the light theme.</extracomment>
        <translation>Verwenden Sie das Lichtanwendungsthema</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="492"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="653"/>
        <source>Dark</source>
        <extracomment>Menu label for the dark visual theme.</extracomment>
        <translation>Dunkel</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="496"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="655"/>
        <source>Use the high-contrast dark application theme</source>
        <extracomment>Tooltip for switching the whole application to the dark theme.</extracomment>
        <translation>Verwenden Sie das kontrastreiche dunkle Anwendungsthema</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="503"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="660"/>
        <source>English</source>
        <extracomment>Language menu entry naming the English user-interface language.</extracomment>
        <translation>Englisch</translation>
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
        <translation>Rückgängig machen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="534"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="695"/>
        <source>Revert the last file-move-operation</source>
        <extracomment>Tooltip for the Undo action that moves the previously moved image back to its original location.</extracomment>
        <translation>Machen Sie den letzten Dateiverschiebungsvorgang rückgängig</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="543"/>
        <source>Could not undo the last file move.</source>
        <extracomment>Fallback status bar error after an undo request failed without a more specific message.</extracomment>
        <translation>Die letzte Dateiverschiebung konnte nicht rückgängig gemacht werden.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="549"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="700"/>
        <source>Redo</source>
        <extracomment>Edit menu action label that reapplies the most recently undone file move.</extracomment>
        <translation>Wiederholen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="551"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="702"/>
        <source>Redo the last file-move-operation (means: undo undo)</source>
        <extracomment>Tooltip for the Redo action. &quot;undo undo&quot; here means reapplied after an Undo.</extracomment>
        <translation>Den letzten Dateiverschiebungsvorgang wiederholen (bedeutet: rückgängig machen und rückgängig machen)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="560"/>
        <source>Could not redo the last file move.</source>
        <extracomment>Fallback status bar error after a redo request failed without a more specific message.</extracomment>
        <translation>Die letzte Dateiverschiebung konnte nicht wiederholt werden.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="567"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="712"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="803"/>
        <source>About Cullendula</source>
        <extracomment>Help menu action label that opens the application&apos;s About dialog.
----------
Title of the application&apos;s About dialog.</extracomment>
        <translation>Über Cullendula</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="569"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="714"/>
        <source>Show the application&apos;s About box</source>
        <extracomment>Tooltip for opening the application&apos;s About dialog.</extracomment>
        <translation>Zeigt das Info-Feld der Anwendung an</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="574"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="719"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="821"/>
        <source>About Qt</source>
        <extracomment>Help menu action label that opens Qt&apos;s built-in About dialog.
----------
Title of the Qt runtime information dialog. Rich-text body of the Qt runtime information dialog. %1 is the Qt version, %2 is the Qt installation prefix path.</extracomment>
        <translation>Über Qt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="576"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="721"/>
        <source>Show the Qt library&apos;s About box</source>
        <extracomment>Tooltip for opening Qt&apos;s built-in About dialog.</extracomment>
        <translation>Zeigt das Info-Feld der Qt-Bibliothek an</translation>
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
        <translation>Hauptsächlich</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="623"/>
        <source>Extensions</source>
        <extracomment>Submenu listing the enabled image filename extensions for directory scanning.</extracomment>
        <translation>Erweiterungen</translation>
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
        <translation>Sprache</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="662"/>
        <source>Use the default English source texts</source>
        <extracomment>Tooltip for switching back to the original English source texts.</extracomment>
        <translation>Verwenden Sie die standardmäßigen englischen Quelltexte</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="669"/>
        <source>Load the German user-interface translation</source>
        <extracomment>Tooltip for loading the German translation file.</extracomment>
        <translation>Laden Sie die deutsche Übersetzung der Benutzeroberfläche</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="676"/>
        <source>Load the Croatian user-interface translation</source>
        <extracomment>Tooltip for loading the Croatian translation file.</extracomment>
        <translation>Laden Sie die kroatische Übersetzung der Benutzeroberfläche</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="683"/>
        <source>Load the Chinese user-interface translation</source>
        <extracomment>Tooltip for loading the Simplified Chinese translation file.</extracomment>
        <translation>Laden Sie die chinesische Übersetzung der Benutzeroberfläche</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="688"/>
        <source>Edit</source>
        <extracomment>Top-level menu for undo and redo actions.</extracomment>
        <translation>Bearbeiten</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="707"/>
        <source>Help</source>
        <extracomment>Top-level menu for About dialogs and other help-related actions.</extracomment>
        <translation>Helfen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="743"/>
        <source>Could not load the selected language.</source>
        <extracomment>Status bar error when loading the requested translation catalog failed.</extracomment>
        <translation>Die ausgewählte Sprache konnte nicht geladen werden.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="801"/>
        <source>Invoked Help|About</source>
        <extracomment>Status bar trace shown when the user opens the application&apos;s About dialog from the Help menu.</extracomment>
        <translation>Hilfe|Info aufgerufen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="805"/>
        <source>Helper program to sort out (&quot;cull&quot;) a collection of pictures in a directory after a nice photo-walk or event.&lt;br&gt;Should work cross-platform.&lt;br&gt;&lt;br&gt;Developed by &lt;a href=&apos;mail@marcelpetrick.it&apos;&gt;mail@marcelpetrick.it&lt;/a&gt;&lt;br&gt;Source code can be found inside the repository at &lt;a href=&apos;https://github.com/marcelpetrick/Cullendula/&apos;&gt;https://github.com/marcelpetrick/Cullendula&lt;/a&gt;&lt;br&gt;Feel free to use and share: GPL v3 :3</source>
        <extracomment>Rich-text body of the application&apos;s About dialog.</extracomment>
        <translation>Hilfsprogramm zum Sortieren („Aussortieren“) einer Sammlung von Bildern in einem Verzeichnis nach einem schönen Foto-Spaziergang oder einer Veranstaltung.&lt;br&gt;Sollte plattformübergreifend funktionieren.&lt;br&gt;&lt;br&gt;Entwickelt von &lt;a href=&apos;mail@marcelpetrick.it&apos;&gt;mail@marcelpetrick.it&lt;/a&gt;&lt;br&gt;Der Quellcode befindet sich im Repository unter &lt;a href=&apos;https://github.com/marcelpetrick/Cullendula/&apos;&gt;https://github.com/marcelpetrick/Cullendula&lt;/a&gt;&lt;br&gt;Sie können es gerne verwenden und teilen: GPL v3 :3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="818"/>
        <source>Invoked Help|About Qt</source>
        <extracomment>Status bar trace shown when the user opens Qt&apos;s About dialog from the Help menu.</extracomment>
        <translation>Aufgerufene Hilfe|Über Qt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="821"/>
        <source>This application currently runs with Qt %1.&lt;br&gt;Qt installation prefix: %2&lt;br&gt;&lt;br&gt;Qt is a cross-platform application framework for building desktop and embedded applications.</source>
        <translation>Diese Anwendung wird derzeit mit Qt %1 ausgeführt.&lt;br&gt;Qt-Installationspräfix: %2&lt;br&gt;&lt;br&gt;Qt ist ein plattformübergreifendes Anwendungsframework zum Erstellen von Desktop- und eingebetteten Anwendungen.</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="23"/>
        <source>Cullendula - your smol helper to pick the best shots</source>
        <comment>Main window title shown before the runtime version suffix is applied from C++ code.</comment>
        <translation>Cullendula – Ihr kleiner Helfer bei der Auswahl der besten Aufnahmen</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="55"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;This label will show the current photo (image-file) in case one is loaded.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Please drag and drop a directory or file on this area.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the large central preview area. It accepts a dropped image file or a directory containing images.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot;font-size:10pt;&quot;&gt;Diese Beschriftung zeigt das aktuelle Foto (Bilddatei) an, falls eines geladen ist.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot;font-size:10pt;&quot;&gt;Bitte ziehen Sie ein Verzeichnis oder eine Datei per Drag &amp; Drop in diesen Bereich.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="64"/>
        <source>drag&amp;drop folder or file to start :)
(loads the enabled extensions from Main -&gt; Extensions, for example *.jpg, *.jpeg, *.png, *.webp)</source>
        <comment>Placeholder text in the central preview area before any image is loaded.</comment>
        <translation>Ziehen Sie den Ordner oder die Datei per Drag&amp;Drop zum Starten :) 
(lädt die aktivierten Erweiterungen von „Haupt“ -&gt; „Erweiterungen“, zum Beispiel *.jpg, *.jpeg, *.png, *.webp)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="131"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the left neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;left&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the left navigation button; it shows the previous image in the current directory order.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Wechseln Sie zum linken Nachbarn (basierend auf der Dateiliste des aktuellen Verzeichnisses).&lt;/p&gt;&lt;p&gt;Als Tastenkombination drücken Sie die Taste „links“. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
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
        <translation>Links</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="164"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified output-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;up&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the Save button. This moves the current image into the app-managed &apos;output&apos; subdirectory, it does not write image edits.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Verschieben Sie das aktuelle Bild in das angegebene Ausgabe-Unterverzeichnis. Wechselt automatisch zum nächsten Foto.&lt;/p&gt;&lt;p&gt;Drücken Sie als Verknüpfung die Taste „Nach oben“. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="167"/>
        <source>save ↑</source>
        <comment>Label of the button that keeps the current image by moving it into the &apos;output&apos; subdirectory.</comment>
        <translation>speichern ↑</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="170"/>
        <source>Up</source>
        <comment>Keyboard shortcut for the Save action; use the standard Up Arrow key sequence.</comment>
        <translation>Hoch</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="197"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified trash-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;down&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the Trash button. This moves the current image into the app-managed &apos;trash&apos; subdirectory, it does not delete it permanently.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Verschiebt das aktuelle Bild in das angegebene Trash-Unterverzeichnis. Wechselt automatisch zum nächsten Foto.&lt;/p&gt;&lt;p&gt;Drücken Sie als Verknüpfung die Taste „Nach unten“. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="200"/>
        <source>trash ↓</source>
        <comment>Label of the button that rejects the current image by moving it into the &apos;trash&apos; subdirectory.</comment>
        <translation>Müll ↓</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="203"/>
        <source>Down</source>
        <comment>Keyboard shortcut for the Trash action; use the standard Down Arrow key sequence.</comment>
        <translation>Runter</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="232"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the right neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;right&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the right navigation button; it shows the next image in the current directory order.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Wechseln Sie zum rechten Nachbarn (basierend auf der Dateiliste des aktuellen Verzeichnisses).&lt;/p&gt;&lt;p&gt;Als Tastenkombination drücken Sie die „rechte“ Taste. Pfeiltaste.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
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
        <translation>Rechts</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="277"/>
        <source>This is the status bar. It will display any kind of information like current position, failure messages, ..</source>
        <comment>Tooltip for the status bar at the bottom of the main window.</comment>
        <translation>Dies ist die Statusleiste. Es werden alle Arten von Informationen wie die aktuelle Position, Fehlermeldungen usw. angezeigt.</translation>
    </message>
</context>
</TS>

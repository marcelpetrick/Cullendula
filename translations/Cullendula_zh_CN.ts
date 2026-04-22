<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>CullendulaFileSystemHandler</name>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="53"/>
        <source>Could not move &apos;%1&apos; to &apos;%2&apos;: %3</source>
        <extracomment>Error message after moving a file into a named subdirectory such as &quot;output&quot;, &quot;trash&quot;, &quot;undo&quot;, or &quot;redo&quot; failed.</extracomment>
        <translation>无法将“%1”移动到“%2”：%3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="58"/>
        <source>Could not prepare &apos;%1&apos; directory at &apos;%2&apos;: %3</source>
        <extracomment>Error message after preparing an application-managed subdirectory such as &quot;output&quot; or &quot;trash&quot; failed.</extracomment>
        <translation>无法在“%2”处准备“%1”目录：%3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="206"/>
        <source>showing %1 of %2</source>
        <extracomment>Status bar message showing the current 1-based image position and the total number of loaded images.</extracomment>
        <translation>显示 %1（共 %2）</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="231"/>
        <source>No undo step is currently available.</source>
        <extracomment>Error shown when the user requests Undo but the undo history is empty.</extracomment>
        <translation>当前没有可用的撤消步骤。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="248"/>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="285"/>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="474"/>
        <source>the filesystem rename operation failed</source>
        <extracomment>Low-level filesystem failure detail inserted into a larger user-visible move error message.</extracomment>
        <translation>文件系统重命名操作失败</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="262"/>
        <source>No redo step is currently available.</source>
        <extracomment>Error shown when the user requests Redo but the redo history is empty.</extracomment>
        <translation>当前没有可用的重做步骤。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="346"/>
        <source>The path &apos;%1&apos; could not be resolved to an existing directory.</source>
        <extracomment>Error after a dropped path or selected path does not resolve to an existing directory on disk.</extracomment>
        <translation>无法将路径“%1”解析为现有目录。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="415"/>
        <source>the path is already occupied by a non-directory filesystem entry</source>
        <extracomment>Failure detail for a managed output directory path that already exists as a regular file or another non-directory entry.</extracomment>
        <translation>该路径已被非目录文件系统条目占用</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="427"/>
        <source>creating the directory failed</source>
        <extracomment>Failure detail for a managed output directory that Qt could not create on disk.</extracomment>
        <translation>创建目录失败</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="434"/>
        <source>the directory is still missing after creation</source>
        <extracomment>Failure detail for a managed output directory that still does not exist after a reported creation attempt.</extracomment>
        <translation>创建后目录仍然丢失</translation>
    </message>
    <message>
        <location filename="../src/CullendulaFileSystemHandler.cpp" line="452"/>
        <source>No current image is available to move.</source>
        <extracomment>Error shown when the user tries to save or trash an image but no current image is loaded.</extracomment>
        <translation>当前没有可移动的图像。</translation>
    </message>
</context>
<context>
    <name>CullendulaMainWindow</name>
    <message>
        <source>Cullendula - your smol helper to pick the best shots</source>
        <translation type="vanished">Cullendula - 您挑选最佳镜头的小帮手</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;This label will show the current photo (image-file) in case one is loaded.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Please drag and drop a directory or file on this area.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;如果加载了一张照片（图像文件），此标签将显示当前照片（图像文件）。&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;请将目录或文件拖放到此区域。&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>drag&amp;drop folder or file to start :)
(loads the enabled extensions from Main -&gt; Extensions, for example *.jpg, *.jpeg, *.png, *.webp)</source>
        <translation type="vanished">拖放文件夹或文件即可开始:)
 （从主 -&gt; 扩展加载启用的扩展，例如 *.jpg、*.jpeg、*.png、*.webp）</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the left neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;left&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;切换到左邻居（基于当前目录的文件列表）。&lt;/p&gt;&lt;p&gt;作为快捷方式，按“左”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>←</source>
        <translation type="vanished">←</translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="vanished">左边</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified output-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;up&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;将当前图片移动到指定的输出子目录中。 将自动切换到下一张照片&lt;/p&gt;&lt;p&gt;作为快捷方式，按“向上”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>save ↑</source>
        <translation type="vanished">保存↑</translation>
    </message>
    <message>
        <source>Up</source>
        <translation type="vanished">向上</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified trash-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;down&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;将当前图片移动到指定的垃圾子目录中。 将自动切换到下一张照片&lt;/p&gt;&lt;p&gt;作为快捷方式，按“向下”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>trash ↓</source>
        <translation type="vanished">垃圾↓</translation>
    </message>
    <message>
        <source>Down</source>
        <translation type="vanished">向下</translation>
    </message>
    <message>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the right neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;right&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="vanished">&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;切换到右邻居（基于当前目录的文件列表）。&lt;/p&gt;&lt;p&gt;作为快捷方式，按“右”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <source>→ </source>
        <translation type="vanished">→</translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="vanished">正确的</translation>
    </message>
    <message>
        <source>This is the status bar. It will display any kind of information like current position, failure messages, ..</source>
        <translation type="vanished">这是状态栏。 它将显示任何类型的信息，如当前位置、故障消息等。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="202"/>
        <source>system is up and running :)</source>
        <extracomment>Status bar message shown once after the main window is fully initialized.</extracomment>
        <translation>系统已启动并运行:)</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="231"/>
        <source>drop current load and let&apos;s see what you dragged?</source>
        <extracomment>Status bar prompt during drag-and-drop after the payload was recognized as file-system URLs.</extracomment>
        <translation>放下当前负载，让我们看看您拖动了什么？</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="271"/>
        <source>The load was not usable! :(</source>
        <extracomment>Status bar error after a drop payload was rejected because it did not contain usable local file URLs.</extracomment>
        <translation>负载无法使用！ :(</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="326"/>
        <source>Could not save the current file.</source>
        <extracomment>Fallback status bar error after moving the current image to the &quot;output&quot; folder failed without a more specific message.</extracomment>
        <translation>无法保存当前文件。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="345"/>
        <source>Could not trash the current file.</source>
        <extracomment>Fallback status bar error after moving the current image to the &quot;trash&quot; folder failed without a more specific message.</extracomment>
        <translation>无法删除当前文件。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="417"/>
        <source>no more valid images found: work maybe finished? :)
drag&amp;drop the next folder or files if you want!</source>
        <extracomment>Center label placeholder when no further images match the current extension filter in the active directory.</extracomment>
        <translation>找不到更多有效图像：工作可能完成了吗？ :)
 如果需要，可以拖放下一个文件夹或文件！</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="423"/>
        <source>no more files</source>
        <extracomment>Status bar message when the current directory no longer contains any matching images to show.</extracomment>
        <translation>没有更多文件</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="430"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="432"/>
        <source>could not load the current image preview</source>
        <extracomment>Error shown both in the center label and the status bar when Qt cannot render the current image preview.</extracomment>
        <translation>无法加载当前图像预览</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="478"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="640"/>
        <source>Enable loading of *.%1 files when opening the next directory</source>
        <extracomment>Tooltip for a checkable menu entry that enables a filename extension such as jpg, png, or webp for future directory scans.</extracomment>
        <translation>打开下一个目录时启用 *.%1 文件的加载</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="484"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="646"/>
        <source>Light</source>
        <extracomment>Menu label for the light visual theme.</extracomment>
        <translation>光</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="488"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="648"/>
        <source>Use the light application theme</source>
        <extracomment>Tooltip for switching the whole application to the light theme.</extracomment>
        <translation>使用轻应用主题</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="492"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="653"/>
        <source>Dark</source>
        <extracomment>Menu label for the dark visual theme.</extracomment>
        <translation>黑暗的</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="496"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="655"/>
        <source>Use the high-contrast dark application theme</source>
        <extracomment>Tooltip for switching the whole application to the dark theme.</extracomment>
        <translation>使用高对比度深色应用程序主题</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="503"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="660"/>
        <source>English</source>
        <extracomment>Language menu entry naming the English user-interface language.</extracomment>
        <translation>英语</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="510"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="667"/>
        <source>Deutsch</source>
        <extracomment>Language menu entry naming the German user-interface language in German.</extracomment>
        <translation>德语</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="517"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="674"/>
        <source>Hrvatski</source>
        <extracomment>Language menu entry naming the Croatian user-interface language in Croatian.</extracomment>
        <translation>赫尔瓦茨基</translation>
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
        <translation>撤消</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="534"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="695"/>
        <source>Revert the last file-move-operation</source>
        <extracomment>Tooltip for the Undo action that moves the previously moved image back to its original location.</extracomment>
        <translation>恢复上次文件移动操作</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="543"/>
        <source>Could not undo the last file move.</source>
        <extracomment>Fallback status bar error after an undo request failed without a more specific message.</extracomment>
        <translation>无法撤消上次文件移动。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="549"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="700"/>
        <source>Redo</source>
        <extracomment>Edit menu action label that reapplies the most recently undone file move.</extracomment>
        <translation>重做</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="551"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="702"/>
        <source>Redo the last file-move-operation (means: undo undo)</source>
        <extracomment>Tooltip for the Redo action. &quot;undo undo&quot; here means reapplied after an Undo.</extracomment>
        <translation>重做上次文件移动操作（意味着：撤消撤消）</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="560"/>
        <source>Could not redo the last file move.</source>
        <extracomment>Fallback status bar error after a redo request failed without a more specific message.</extracomment>
        <translation>无法重做上次文件移动。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="567"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="712"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="803"/>
        <source>About Cullendula</source>
        <extracomment>Help menu action label that opens the application&apos;s About dialog.
----------
Title of the application&apos;s About dialog.</extracomment>
        <translation>关于卡伦杜拉</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="569"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="714"/>
        <source>Show the application&apos;s About box</source>
        <extracomment>Tooltip for opening the application&apos;s About dialog.</extracomment>
        <translation>显示应用程序的“关于”框</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="574"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="719"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="821"/>
        <source>About Qt</source>
        <extracomment>Help menu action label that opens Qt&apos;s built-in About dialog.
----------
Title of the Qt runtime information dialog. Rich-text body of the Qt runtime information dialog. %1 is the Qt version, %2 is the Qt installation prefix path.</extracomment>
        <translation>关于Qt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="576"/>
        <location filename="../src/CullendulaMainWindow.cpp" line="721"/>
        <source>Show the Qt library&apos;s About box</source>
        <extracomment>Tooltip for opening Qt&apos;s built-in About dialog.</extracomment>
        <translation>显示 Qt 库的“关于”框</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="614"/>
        <source>Cullendula</source>
        <extracomment>Main window title; the version suffix is appended separately in code.</extracomment>
        <translation>库伦杜拉</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="618"/>
        <source>Main</source>
        <extracomment>Top-level menu containing application settings such as extensions, style, and language.</extracomment>
        <translation>主要的</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="623"/>
        <source>Extensions</source>
        <extracomment>Submenu listing the enabled image filename extensions for directory scanning.</extracomment>
        <translation>扩展</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="628"/>
        <source>Style</source>
        <extracomment>Submenu for switching between visual themes.</extracomment>
        <translation>风格</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="633"/>
        <source>Language</source>
        <extracomment>Submenu for switching the user-interface language.</extracomment>
        <translation>语言</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="662"/>
        <source>Use the default English source texts</source>
        <extracomment>Tooltip for switching back to the original English source texts.</extracomment>
        <translation>使用默认的英文源文本</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="669"/>
        <source>Load the German user-interface translation</source>
        <extracomment>Tooltip for loading the German translation file.</extracomment>
        <translation>加载德语用户界面翻译</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="676"/>
        <source>Load the Croatian user-interface translation</source>
        <extracomment>Tooltip for loading the Croatian translation file.</extracomment>
        <translation>加载克罗地亚语用户界面翻译</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="683"/>
        <source>Load the Chinese user-interface translation</source>
        <extracomment>Tooltip for loading the Simplified Chinese translation file.</extracomment>
        <translation>加载中文用户界面翻译</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="688"/>
        <source>Edit</source>
        <extracomment>Top-level menu for undo and redo actions.</extracomment>
        <translation>编辑</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="707"/>
        <source>Help</source>
        <extracomment>Top-level menu for About dialogs and other help-related actions.</extracomment>
        <translation>帮助</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="743"/>
        <source>Could not load the selected language.</source>
        <extracomment>Status bar error when loading the requested translation catalog failed.</extracomment>
        <translation>无法加载所选语言。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="801"/>
        <source>Invoked Help|About</source>
        <extracomment>Status bar trace shown when the user opens the application&apos;s About dialog from the Help menu.</extracomment>
        <translation>调用帮助|关于</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="805"/>
        <source>Helper program to sort out (&quot;cull&quot;) a collection of pictures in a directory after a nice photo-walk or event.&lt;br&gt;Should work cross-platform.&lt;br&gt;&lt;br&gt;Developed by &lt;a href=&apos;mail@marcelpetrick.it&apos;&gt;mail@marcelpetrick.it&lt;/a&gt;&lt;br&gt;Source code can be found inside the repository at &lt;a href=&apos;https://github.com/marcelpetrick/Cullendula/&apos;&gt;https://github.com/marcelpetrick/Cullendula&lt;/a&gt;&lt;br&gt;Feel free to use and share: GPL v3 :3</source>
        <extracomment>Rich-text body of the application&apos;s About dialog.</extracomment>
        <translation>帮助程序，用于在一次精彩的拍照散步或活动后整理（“剔除”）目录中的一组图片。&lt;br&gt;应该跨平台工作。&lt;br&gt;&lt;br&gt;由 &lt;a href=&apos;mail@marcelpetrick.it&apos;&gt;mail@marcelpetrick.it&lt;/a&gt;&lt;br&gt;可以在存储库中找到源代码：&lt;a href=&apos;https://github.com/marcelpetrick/Cullendula/&apos;&gt;https://github.com/marcelpetrick/Cullendula&lt;/a&gt;&lt;br&gt;随意使用和分享：GPL v3 :3</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="818"/>
        <source>Invoked Help|About Qt</source>
        <extracomment>Status bar trace shown when the user opens Qt&apos;s About dialog from the Help menu.</extracomment>
        <translation>调用帮助|关于Qt</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.cpp" line="821"/>
        <source>This application currently runs with Qt %1.&lt;br&gt;Qt installation prefix: %2&lt;br&gt;&lt;br&gt;Qt is a cross-platform application framework for building desktop and embedded applications.</source>
        <translation>此应用程序当前使用 Qt %1 运行。&lt;br&gt;Qt 安装前缀：%2&lt;br&gt;&lt;br&gt;Qt 是用于构建桌面和嵌入式应用程序的跨平台应用程序框架。</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="23"/>
        <source>Cullendula - your smol helper to pick the best shots</source>
        <comment>Main window title shown before the runtime version suffix is applied from C++ code.</comment>
        <translation>Cullendula - 您挑选最佳镜头的小帮手</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="55"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;This label will show the current photo (image-file) in case one is loaded.&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;Please drag and drop a directory or file on this area.&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the large central preview area. It accepts a dropped image file or a directory containing images.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;如果加载了一张照片（图像文件），此标签将显示当前照片（图像文件）。&lt;/span&gt;&lt;/p&gt;&lt;p&gt;&lt;span style=&quot; font-size:10pt;&quot;&gt;请将目录或文件拖放到此区域。&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="64"/>
        <source>drag&amp;drop folder or file to start :)
(loads the enabled extensions from Main -&gt; Extensions, for example *.jpg, *.jpeg, *.png, *.webp)</source>
        <comment>Placeholder text in the central preview area before any image is loaded.</comment>
        <translation>拖放文件夹或文件即可开始:)
 （从主 -&gt; 扩展加载启用的扩展，例如 *.jpg、*.jpeg、*.png、*.webp）</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="131"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the left neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;left&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the left navigation button; it shows the previous image in the current directory order.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;切换到左邻居（基于当前目录的文件列表）。&lt;/p&gt;&lt;p&gt;作为快捷方式，按“左”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
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
        <translation>左边</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="164"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified output-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;up&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the Save button. This moves the current image into the app-managed &apos;output&apos; subdirectory, it does not write image edits.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;将当前图片移动到指定的输出子目录中。 将自动切换到下一张照片&lt;/p&gt;&lt;p&gt;作为快捷方式，按“向上”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="167"/>
        <source>save ↑</source>
        <comment>Label of the button that keeps the current image by moving it into the &apos;output&apos; subdirectory.</comment>
        <translation>保存↑</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="170"/>
        <source>Up</source>
        <comment>Keyboard shortcut for the Save action; use the standard Up Arrow key sequence.</comment>
        <translation>向上</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="197"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Move the current picture into the specified trash-subdirectory. Will automatically switch to the photo which is next&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;down&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the Trash button. This moves the current image into the app-managed &apos;trash&apos; subdirectory, it does not delete it permanently.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;将当前图片移动到指定的垃圾子目录中。 将自动切换到下一张照片&lt;/p&gt;&lt;p&gt;作为快捷方式，按“向下”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="200"/>
        <source>trash ↓</source>
        <comment>Label of the button that rejects the current image by moving it into the &apos;trash&apos; subdirectory.</comment>
        <translation>垃圾↓</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="203"/>
        <source>Down</source>
        <comment>Keyboard shortcut for the Trash action; use the standard Down Arrow key sequence.</comment>
        <translation>向下</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="232"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Switch to the right neighbour (based on the file-list of the current directory).&lt;/p&gt;&lt;p&gt;As shortcut press the &amp;quot;right&amp;quot; arrow key.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <comment>Tooltip for the right navigation button; it shows the next image in the current directory order.</comment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;切换到右邻居（基于当前目录的文件列表）。&lt;/p&gt;&lt;p&gt;作为快捷方式，按“右”键 箭头键。&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
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
        <translation>正确的</translation>
    </message>
    <message>
        <location filename="../src/CullendulaMainWindow.ui" line="277"/>
        <source>This is the status bar. It will display any kind of information like current position, failure messages, ..</source>
        <comment>Tooltip for the status bar at the bottom of the main window.</comment>
        <translation>这是状态栏。 它将显示任何类型的信息，如当前位置、故障消息等。</translation>
    </message>
</context>
</TS>

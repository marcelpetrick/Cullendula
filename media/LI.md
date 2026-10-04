Porting to Qt 6.12 - obey the CRA

At the end of September QT 6.12 was released into the wild. And since this is a much anticipated LTS-version (last one was 6.8) I started to port my FOSS projects.
Cullendula, since it is a tiny project, but quite old (~8 years) went though a lot of change. It is a helper to cull unwanted photos from a weekend session (aka: just filter the best ones out). Boring old fashioned widgets with some C++ backend code: read the directory, load image-files, move them to different directories, some UI-themes and i18n, nothing fancy.
But that makes it an ideal candidate to try out new Qt releases.

The work was done in less than 30 minutes. Thanks to guardrails like 98% test coverage and CI pipelines. Supported by the use of modern dependencies like cmake 4.4.3, C++23, .. which prevented that I had to modernize not just Qt, but other things as well.

So what do we get now? A LTS-release, which promises 5 years of support, which means we should be well defended against cybersecurity vulnerabilities and software flaws (CVE). Again two releases for Windows (Executable) and Linux (AppImage).
And a resulting migration guide. Which is actually the more informative part for me - because I would like to know what we have to change to port existing software.

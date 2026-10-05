Porting to 𝐐𝐭 𝟔.𝟏𝟐 - obey the 𝐂𝐑𝐀

At the end of September, Qt 6.12 was released into the wild. And since this is a much-anticipated LTS version (the last one was 6.8), I started to port my FOSS projects.
𝐂𝐮𝐥𝐥𝐞𝐧𝐝𝐮𝐥𝐚, since it is a tiny project but quite old (~8 years), went through a lot of changes. It is a helper to cull unwanted photos from a weekend session (aka: just filter the best ones out). Boring old-fashioned widgets with some C++ backend code: read the directory, load image files, move them to different directories, some UI themes and 𝐢𝟏𝟖𝐧, nothing fancy.
But that makes it an ideal candidate to try out new Qt releases.

The work was done in less than 30 minutes, thanks to guardrails like 98% test coverage and CI pipelines. This was supported by the use of modern dependencies like 𝐂𝐌𝐚𝐤𝐞 𝟒.𝟒.𝟑, 𝐂++𝟐𝟑, etc., which prevented me from having to modernize not just Qt, but other things as well.

So what do we get now? An LTS release, which promises 𝟓 𝐲𝐞𝐚𝐫𝐬 𝐨𝐟 𝐬𝐮𝐩𝐩𝐨𝐫𝐭, which means we should be well defended against cybersecurity vulnerabilities and software flaws (CVEs). Which is part of the CRA. To resolve the pending question what does CRA even mean: this is the 𝐂𝐲𝐛𝐞𝐫 𝐑𝐞𝐬𝐢𝐥𝐢𝐞𝐧𝐜𝐞 𝐀𝐜𝐭, where the first major enforcement milestone took effect on 11th September 2026. Vendors of software-products are responsible to deliver maintenance
Again, two releases for Windows (𝐞𝐱𝐞𝐜𝐮𝐭𝐚𝐛𝐥𝐞) and Linux (𝐀𝐩𝐩𝐈𝐦𝐚𝐠𝐞).
And a resulting 𝐦𝐢𝐠𝐫𝐚𝐭𝐢𝐨𝐧 𝐠𝐮𝐢𝐝𝐞, which is actually the more informative part for me - because I like to know what we have to change to 𝐩𝐨𝐫𝐭 𝐞𝐱𝐢𝐬𝐭𝐢𝐧𝐠 𝐬𝐨𝐟𝐭𝐰𝐚𝐫𝐞.

Fork the 2.6k LoC and 2.7k lines of tests at GitHub: https://github.com/marcelpetrick/Cullendula/

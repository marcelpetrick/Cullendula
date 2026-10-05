#!/usr/bin/env bash

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
QT_PATHS_COMMAND="${QT_PATHS_COMMAND:-$(command -v qtpaths6 || true)}"

if [[ -z "${DISPLAY:-}" ]]; then
    echo "A graphical desktop session is required for decorated window captures." >&2
    exit 1
fi

if [[ -z "${QT_PATHS_COMMAND}" ]]; then
    echo "qtpaths6 was not found. Put the pinned Qt 6.12.0 bin directory on PATH." >&2
    exit 1
fi

QT_ROOT="$(${QT_PATHS_COMMAND} --query QT_INSTALL_PREFIX)"
QT_VERSION="$(${QT_PATHS_COMMAND} --qt-version)"
if [[ "${QT_VERSION}" != "6.12.0" ]]; then
    echo "The walkthrough requires Qt 6.12.0, but qtpaths6 reports ${QT_VERSION}." >&2
    exit 1
fi

for command in cmake c++ fc-match ffmpeg ffprobe magick pkg-config spectacle; do
    if ! command -v "${command}" >/dev/null 2>&1; then
        echo "Required command not found: ${command}" >&2
        exit 1
    fi
done

CAPTION_FONT="$(fc-match -f '%{file}' 'Noto Sans' | head -1)"
if [[ ! -f "${CAPTION_FONT}" ]]; then
    echo "Could not resolve a usable sans-serif caption font." >&2
    exit 1
fi

WORK_DIRECTORY="$(mktemp -d /tmp/cullendula-walkthrough.XXXXXX)"
cleanup() {
    if [[ "${WALKTHROUGH_KEEP_WORK:-0}" == "1" ]]; then
        echo "Kept walkthrough work directory: ${WORK_DIRECTORY}"
        return
    fi
    rm -rf "${WORK_DIRECTORY}"
}
trap cleanup EXIT

export PATH="${QT_ROOT}/bin:${PATH}"
export PKG_CONFIG_PATH="${QT_ROOT}/lib/pkgconfig${PKG_CONFIG_PATH:+:${PKG_CONFIG_PATH}}"

cmake -S "${PROJECT_ROOT}" -B "${PROJECT_ROOT}/build" -DCMAKE_PREFIX_PATH="${QT_ROOT}"
cmake --build "${PROJECT_ROOT}/build" --target Cullendula --parallel "$(nproc)"

DRIVER="${WORK_DIRECTORY}/CullendulaWalkthrough"
c++ -std=c++23 -fPIC \
    "${PROJECT_ROOT}/scripts/CullendulaWalkthrough.cpp" \
    "${PROJECT_ROOT}/build/src/libCullendulaLib.a" \
    -I"${PROJECT_ROOT}/src" \
    $(pkg-config --cflags --libs Qt6Widgets Qt6Gui Qt6Core) \
    -o "${DRIVER}"

RESOURCE_DESCRIPTION="${WORK_DIRECTORY}/walkthrough-translations.qrc"
cat >"${RESOURCE_DESCRIPTION}" <<EOF
<RCC>
  <qresource prefix="/i18n">
    <file alias="Cullendula_de.qm">${PROJECT_ROOT}/build/Cullendula_de.qm</file>
    <file alias="Cullendula_hr.qm">${PROJECT_ROOT}/build/Cullendula_hr.qm</file>
    <file alias="Cullendula_zh_CN.qm">${PROJECT_ROOT}/build/Cullendula_zh_CN.qm</file>
  </qresource>
</RCC>
EOF

TRANSLATION_RESOURCE="${WORK_DIRECTORY}/walkthrough-translations.rcc"
"${QT_ROOT}/libexec/rcc" -binary "${RESOURCE_DESCRIPTION}" -o "${TRANSLATION_RESOURCE}"

WINDOW_SHELL="${WORK_DIRECTORY}/window-shell.png"
SHELL_FRAME_DIRECTORY="${WORK_DIRECTORY}/shell-frames"
mkdir -p "${SHELL_FRAME_DIRECTORY}"
LD_LIBRARY_PATH="${QT_ROOT}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}" \
    QT_QPA_PLATFORM=xcb \
    "${DRIVER}" "${PROJECT_ROOT}/testItemFolder" "${TRANSLATION_RESOURCE}" shell "${SHELL_FRAME_DIRECTORY}" \
    >"${WORK_DIRECTORY}/shell.log" 2>&1 &
shell_pid=$!
sleep 1
env -u LD_LIBRARY_PATH -u QT_PLUGIN_PATH -u QT_ROOT_DIR -u QT_QPA_PLATFORM \
    spectacle --activewindow --background --nonotify --output "${WINDOW_SHELL}"
kill "${shell_pid}" 2>/dev/null || true
wait "${shell_pid}" 2>/dev/null || true

if [[ ! -s "${WINDOW_SHELL}" ]]; then
    echo "Spectacle did not capture the decorated window shell." >&2
    exit 1
fi

render_variant() {
    local variant="$1"
    local output="$2"
    local demo_directory="${WORK_DIRECTORY}/demo-${variant}"
    local frame_directory="${WORK_DIRECTORY}/frames-${variant}"
    local decorated_directory="${WORK_DIRECTORY}/decorated-${variant}"
    local concat_file="${WORK_DIRECTORY}/${variant}-frames.txt"

    mkdir -p "${demo_directory}" "${frame_directory}" "${decorated_directory}"
    cp "${PROJECT_ROOT}"/testItemFolder/* "${demo_directory}/"

    LD_LIBRARY_PATH="${QT_ROOT}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}" \
        QT_QPA_PLATFORM=xcb \
        "${DRIVER}" "${demo_directory}" "${TRANSLATION_RESOURCE}" "${variant}" "${frame_directory}" \
        >"${WORK_DIRECTORY}/${variant}.log" 2>&1 || {
        cat "${WORK_DIRECTORY}/${variant}.log" >&2
        return 1
    }

    local frame_index
    frame_index="$(find "${frame_directory}" -maxdepth 1 -name '*.png' | wc -l)"
    if [[ "${frame_index}" -lt 10 ]]; then
        echo "Too few application frames were captured for ${variant}: ${frame_index}" >&2
        return 1
    fi

    local shell_width shell_height client_width client_height client_x client_y
    read -r shell_width shell_height < <(magick identify -format '%w %h\n' "${WINDOW_SHELL}")
    read -r client_width client_height < <(magick identify -format '%w %h\n' "$(find "${frame_directory}" -maxdepth 1 -name '*.png' | sort | head -1)")
    client_x=$(((shell_width - client_width) / 2))
    client_y=$(((shell_height - client_height) / 2))
    if ((client_x < 0 || client_y < 0)); then
        echo "The captured window shell is smaller than the application content." >&2
        return 1
    fi

    local frame
    for frame in "${frame_directory}"/*.png; do
        local caption frame_name
        frame_name="$(basename "${frame}")"
        case "${frame_name}" in
            *-intro.png) caption="Drop a photo folder to begin" ;;
            *-session-loaded.png) caption="Load every supported photo in the session" ;;
            *-browse-next.png) caption="Browse the session in either direction" ;;
            *-keep.png) caption="Keep a favorite in the output folder" ;;
            *-reject.png) caption="Move a rejected shot to trash" ;;
            *-undo.png) caption="Undo a file move safely" ;;
            *-redo.png) caption="Redo it when the decision was right" ;;
            *-themes-menu.png) caption="Choose a light, dark, or purple theme" ;;
            *-theme-dark.png) caption="Preview the high-contrast dark theme" ;;
            *-theme-purple.png) caption="Add a brief splash of purple" ;;
            *-languages-menu.png) caption="Switch the interface language at runtime" ;;
            *-language-german.png) caption="German interface — no restart needed" ;;
            *-language-chinese.png) caption="Chinese interface — no restart needed" ;;
            *-formats.png) caption="Select the image formats to include" ;;
            *-help-menu.png) caption="Open application and Qt information" ;;
            *-about.png) caption="Review Cullendula version and project details" ;;
            *-about-qt.png) caption="Confirm the application is running on Qt 6.12" ;;
            *-finished.png) caption="Cullendula — the keepers are ready" ;;
            *) caption="Cullendula photo culling workflow" ;;
        esac
        local decorated_frame="${decorated_directory}/${frame_name}"

        if [[ "${variant}" == "readme" ]]; then
            magick -size "${shell_width}x${shell_height}" xc:'#fff1d6' \
                "${frame}" -geometry "+${client_x}+${client_y}" -composite \
                "${WINDOW_SHELL}" -composite \
                -resize '960x790>' -gravity north -extent 1000x860 \
                -font "${CAPTION_FONT}" -fill '#1f252d' -pointsize 27 \
                -gravity south -annotate +0+19 "${caption}" \
                "${decorated_frame}"
        else
            magick -size "${shell_width}x${shell_height}" xc:'#fff1d6' \
                "${frame}" -geometry "+${client_x}+${client_y}" -composite \
                "${WINDOW_SHELL}" -composite \
                -resize '900x720>' -gravity center -extent 1080x1080 \
                -fill '#e85d75' -draw 'rectangle 0,0 1080,17' \
                -fill '#663399' -draw 'rectangle 0,1063 1080,1080' \
                -font "${CAPTION_FONT}" -fill '#663399' -pointsize 52 \
                -gravity north -annotate +0+38 'CULLENDULA' \
                -font "${CAPTION_FONT}" -fill '#1f252d' -pointsize 29 \
                -gravity north -annotate +0+102 'Cull a photo session in under 20 seconds' \
                -font "${CAPTION_FONT}" -fill '#1f252d' -pointsize 35 \
                -gravity south -annotate +0+142 "${caption}" \
                -font "${CAPTION_FONT}" -fill '#e85d75' -pointsize 25 \
                -gravity south -annotate +0+68 'github.com/marcelpetrick/Cullendula' \
                "${decorated_frame}"
        fi
    done

    : >"${concat_file}"
    local frame_duration
    if [[ "${variant}" == "readme" ]]; then
        frame_duration=0.78
    else
        frame_duration=0.88
    fi
    for frame in "${decorated_directory}"/*.png; do
        printf "file '%s'\nduration %s\n" "${frame}" "${frame_duration}" >>"${concat_file}"
    done
    printf "file '%s'\n" "$(find "${decorated_directory}" -maxdepth 1 -name '*.png' | sort | tail -1)" >>"${concat_file}"

    ffmpeg -hide_banner -loglevel error -y -f concat -safe 0 -i "${concat_file}" \
        -filter_complex "fps=10,split[frames][palette_input];[palette_input]palettegen=max_colors=160:stats_mode=diff[palette];[frames][palette]paletteuse=dither=bayer:bayer_scale=3:diff_mode=rectangle" \
        -loop 0 "${output}"

    local duration
    duration="$(ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 "${output}")"
    awk -v duration="${duration}" 'BEGIN { if (duration > 20.0) exit 1 }'
    echo "Generated ${output} (${duration}s)"
}

case "${1:-all}" in
    all)
        render_variant readme "${PROJECT_ROOT}/media/cullendula_walkthrough.gif"
        render_variant social "${PROJECT_ROOT}/media/cullendula_walkthrough_social.gif"
        ;;
    readme)
        render_variant readme "${PROJECT_ROOT}/media/cullendula_walkthrough.gif"
        ;;
    social)
        render_variant social "${PROJECT_ROOT}/media/cullendula_walkthrough_social.gif"
        ;;
    *)
        echo "Usage: $0 [all|readme|social]" >&2
        exit 2
        ;;
esac

#!/bin/bash
:>/usr/share/applications/reinovo_control.desktop
echo  '[Desktop Entry]' >>/usr/share/applications/reinovo_control.desktop
echo  'Encoding=UTF-8' >>/usr/share/applications/reinovo_control.desktop
echo  'Name=ReinovoControl' >>/usr/share/applications/reinovo_control.desktop
echo  'Name[zh_CN]=ReinovoControl' >>/usr/share/applications/reinovo_control.desktop
echo  'Comment=Thunderbird Webmail Client' >>/usr/share/applications/reinovo_control.desktop

path=$(readlink -f "$(dirname "$0")")
#$exec = "Exec="${path}"/reinovo_control.sh"
echo  "Exec="$path"/reinovo_control.sh" >>/usr/share/applications/reinovo_control.desktop
#$icon = "Icon="${path}"/reinovo_control.png"
echo  "Icon="$path"/reinovo_control.png" >>/usr/share/applications/reinovo_control.desktop

echo  'Terminal=true' >>/usr/share/applications/reinovo_control.desktop
echo  'StartupNotify=false' >>/usr/share/applications/reinovo_control.desktop
echo  'Type=Application' >>/usr/share/applications/reinovo_control.desktop
echo  'Categories=Application;Communication' >>/usr/share/applications/reinovo_control.desktop

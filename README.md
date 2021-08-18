# Steam ACF to JSON

This script is a quick solution to enable ACF's to be queried as if it were JSON. It has application in trivialising version checking, obtaining branches available and various other tools.

Linux Support: x86, x86_x64, arm, armhf

Windows Support: x86, x86_x64

# Examples
### File examples - Get buildid
> acf_to_json test_appmanifest_file.acf | jq -r .AppState.buildid

```1110931```

> cat test_appmanifest_steamcmd.acf | acf_to_json | jq -r ".[\\"4940\\"].depots.branches.public.buildid"

```2660979```

### Steamcmd example - Get buildid
> ./steamcmd/steamcmd.sh +login anonymous +app_info_update 1 +app_info_print "740" +app_info_print "740" +quit | acf_to_json | jq -r ".[\\"740\\"].depots.branches.public.buildid"

```2735003```

### Steamcmd example - Get all branches [branche buildid timestamp_updated]
> ./steamcmd/steamcmd.sh +login anonymous +app_info_update 1 +app_info_print "740" +app_info_print "740" +quit | acf_to_json | jq -r ".[\\"740\\"].depots.branches | to_entries[] | [.key, .value.buildid, .value.timeupdated, .value.pwdrequired] | @tsv"

```1.36.3.1  2646665 1524265990

1.36.3.2  2708674 1525113411

1.36.3.3  2735003 1525131007

dprs      2479775 1517267346      1

public    2735003 1525130931
```

# Linux Setup
### Clone repository in terminal (please install git beforehand)
> git clone https://github.com/Turbine1991/cpp_steam_acf_to_json.git && cd cpp_steam_acf_to_json

### Install binary globally [x86, x86_x64, arm, armhf]
> su && cp acf_to_json_x86 /usr/local/bin/acf_to_json && cp jq_x86 /usr/local/bin/jq && exit

### Download steamcmd locally for example
> wget "https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz" -P steamcmd && tar -zxf steamcmd/steamcmd_linux.tar.gz -C steamcmd

# Windows Setup
Install GIT https://git-scm.com/downloads

> git clone https://github.com/Turbine1991/cpp_steam_acf_to_json.git && cd cpp_steam_acf_to_json

### Install binary globally [x86, x86_x64]
Open CMD as Administrator and `cd` to cloned path

> mkdir C:\bin

> setx PATH "%path%;C:\bin" && PATH="%path%;C:\bin"

> copy /y acf_to_json_x86.exe C:\bin\acf_to_json.exe && copy /y jq_x86.exe C:\bin\jq.exe


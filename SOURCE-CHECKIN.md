# Application source check-in

This branch contains all 27 application/shared-code folders under
`development/exe/apex`, the shared Visual Studio property sheets, project and
solution files, tests, and application resources (including CAD/geometry assets).
It also includes the Basket Crane 1 live position-refresh fix.

Build outputs, vendor libraries, databases, logs, caches, machine-specific user
settings, and live INI files are excluded from this check-in. Existing repository
history and previously tracked files have not been removed.

## Local configuration

Copy an application's `.ini.example` to the corresponding `.ini` filename and
provide credentials locally. Examples use Test mode where an environment mode
setting exists; configure the intended database and PLC endpoints before use.
Basket Crane 1 requires its local INI alongside the executable for build copying
and installation packaging. Do not commit the populated INI files.

Older applications previously embedded database passwords in their C++ source.
The repository copies now read those passwords from environment variables using
Qt `qgetenv`. `credential-environment-variables.json` maps each affected file to
the required environment-variable names. Set them securely in the application's
launch environment. No password values are stored in that mapping. The original
local production source and its credentials were not edited by this check-in.

## Build dependencies and validation

The existing projects require the separately installed Qt 5.7 static x32 runtime,
Windows/Visual Studio toolchain, and proprietary/vendor libraries expected by the
shared property sheets. These dependencies are not bundled by this check-in.

Basket Crane 1's Debug application build passed locally as 5.6.36. Its assignment
refresh regression tests passed from this branch with the installed Qt runtime.
The other application projects have not all been rebuilt. This is a source
check-in, not a deployment to any production computer.

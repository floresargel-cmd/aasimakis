# Apex Basket Crane MSI installer

This WiX project creates a standard per-machine Windows Installer package for
the x86 Apex Basket Crane application.

## Build

From a Visual Studio developer PowerShell prompt:

```powershell
msbuild .\ApexBasketCrane.Installer.wixproj /restore /p:Configuration=Release /p:Platform=win32
```

The application project is a project reference and builds first. The MSI is
written to `bin\Release\ApexBasketCrane-Release-x86.msi`.

Each application build increments the `major.minor.build` value in
`..\build-version.txt`. The generated value is compiled into the application
and then read by this WiX project for the MSI `ProductVersion`, keeping the
on-screen version and Add/Remove Programs version identical.

## Installed resources

- Application files under
  `%ProgramFiles(x86)%\Lorval Capital Management\Apex Basket Crane`
- Start Menu shortcut
- Desktop shortcut
- Add/Remove Programs registration

The existing `basketCrane.ini` is not overwritten by repair or upgrade. The
installer requires administrator elevation. Database DSNs and other external
plant dependencies are intentionally not created by the MSI.

Before production distribution, sign the application executable and MSI with
the organization's code-signing certificate.

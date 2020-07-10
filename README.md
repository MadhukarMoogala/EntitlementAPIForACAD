# Using Entitlement API from AutoCAD

If you have an app on the [Autodesk App Store](https://apps.autodesk.com/en) then you might want to check if the user actually paid for and downloaded your app from the store or just copied it from someone else's computer. This what the Entitlement API can help you with.

The **Entitlement API** is a simple **RESTful API** where you just need to send an **HTTP** **GET** request to the **App Store** server.

This application is dependent on 

### Prerequisites

Launch VS 2019 Developer Command Prompt or `call <VS2019Path>Common7\Tools\VsDevCmd.bat`

Follow below steps.

```

git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
msbuild toolsrc\vcpkg.sln /t:Build /p:Configuration=Release;Platfrom=x64
xcopy  toolsrc\msbuild.x64.release\vcpkg.exe . /y
vcpkg integrate install
vcpkg install restclient-cpp
vcpkg install jsoncpp

```

Nota Bene: 

`vcpkg integrate install`  applies user-wide integration for this vcpkg root.
All C++ projects can now #include any installed libraries.
Linking will be handled automatically.
Installing new libraries will make them instantly available.

You can now simply use File -> New Project in Visual Studio 2017 or Visual Studio 2019 and the library will be automatically available.

![VcPkg](D:\Work\Arxprojects\2021\EntitlementAPI\VcPkg.png)

###  Build 

```
git clone https://github.com/MadhukarMoogala/EntitlementAPIForACAD.git
cd EntitlementAPIForACAD
set ArxSDKDir="Path of ObjectARX"
set AcadDir="Path of AutoCAD 2021"
msbuild EntitlementAPI.sln /t:build /p:Configuration=Release;Platform=x64
```

Or Interactively open the file Autodesk.arx-2021.props
- Change the property <ArxSdkDir></ArxSdkDir> with ObjectARX  SDK Path
- Change the property <AcadDir></AcadDir> with ObjectARX  SDK Path










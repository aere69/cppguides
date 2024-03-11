# Dynamic Link Libary

This poject creates a DLL and a client application to use it.
It is created using Visual Studio.

## DLL Project

Create a new C++ project, set platform to Windows and Project Type Library.
From Project Types select Dynamic Link Library (DLL).

Add a header file to the project.

Notice the preprocessor statements at the top of the file. The new project template for a DLL project adds <PROJECTNAME>_EXPORTS to the defined preprocessor macros.

When the <PROJECTNAME>_EXPORTS macro is defined, the <PROJECTNAME>_API macro sets the __declspec(dllexport) modifier on the function declarations. This modifier tells the compiler and linker to export a function or variable from the DLL for use by other applications. When <PROJECTNAME>_EXPORTS is undefined, for example, when the header file is included by a client application, <PROJECTNAME>_API applies the __declspec(dllimport) modifier to the declarations. This modifier optimizes the import of the function or variable in an application. For more information, see [dllexport, dllimport](https://learn.microsoft.com/en-us/cpp/cpp/dllexport-dllimport?view=msvc-170).

## Client Project

Create a new C++ project, set platform to Windows and Project Type Console.
From Project Types select Console App.

To call the Library functions in your source code, your project must include the Library.h file. 
You could copy this header file into your client app project, then add it to the project as an existing item. This method can be a good choice for third-party libraries. However, if you're working on the code for your DLL and your client at the same time, the header files could get out of sync. To avoid this issue, set the **Additional Include Directories** path in your project to include the path to the original header.

### Add the DLL header to your include path

Right-click on the Client node in Solution Explorer to open the Property Pages dialog.
In the Configuration drop-down box, select All Configurations.
In the left pane, select Configuration Properties > C/C++ > General.
In the property pane, select the drop-down control next to the Additional Include Directories edit box, and then choose Edit.

### Add the DLL import library to your project

Right-click on the Client node in Solution Explorer and choose Properties to open the Property Pages dialog.
In the Configuration drop-down box, select All Configurations. It ensures that any property changes apply to both Debug and Release builds.
In the left pane, select Configuration Properties > Linker > Input. In the property pane, select the drop-down control next to the Additional Dependencies edit box, and then choose Edit.

Go back to the Property Pages dialog box.
In the left pane, select Configuration Properties > Linker > General. In the property pane, select the drop-down control next to the Additional Library Directories edit box, and then choose Edit.

### copy the DLL in a post-build event

Right-click on the Client node in Solution Explorer and choose Properties to open the Property Pages dialog.
In the Configuration drop-down box, select All Configurations.
In the left pane, select Configuration Properties > Build Events > Post-Build Event.
In the property pane, select the edit control in the Command Line field. If you followed the directions to put your client project in a separate solution from the DLL project, then enter this command:

```
xcopy /y /d "..\..\MathLibrary\$(IntDir)MathLibrary.dll" "$(OutDir)"
```

If your DLL and client projects are in other directories, change the relative path to the DLL to match.

[More Info](https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170)
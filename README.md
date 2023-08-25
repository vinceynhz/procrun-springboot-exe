# procrun-springboot-exe
Run a Spring Boot application using Apache procrun using the exe start mode.

As of 2023, for Spring Boot applications (currently using 2.7.9) built with the spring-boot-plugin, an option to run as a Windows service could be to use procrun's `StartMode` as `exe` instead of the other options.

This is conceptually a similar approach to that of Unix/Linux using init.d services providing a script.

The whole solution includes precompiling a couple of executables to start/stop the java application, and let Spring write the PID to a file. Let's go to it!

# The Spring application #
The only change here is on the `main` method. Use `ApplicationPidFileWriter` to  write the PID (Process Id) to a file that we'll use later to shutdown the application ([ref][1]).

```java
//... main method within a class that uses all the annotations you need.
public static void main(String[] args) {
  SpringApplicationBuilder app = new SpringApplicationBuilder(ServiceMainApp.class);
  app.build().addListeners(new ApplicationPidFileWriter("./shutdown.pid"));
  app.run(args);
}
```

# The start/stop executables #

I chose C since it's the easiest way to get executables for whichever platform you will run your java application, but you can just as easily write them in any other language that compiles to an executable.

Note on the C code that we are passing to java -jar all of the parameters. This will be important.

Compile to `start.exe`.

**stop.c**

**Note:** Unfortunately, in Windows we have to use `/F` option to forcefully shutdown the application, because Java doesn't handle correctly the `WM_CLOSE` signal that `taskkill` sends ([ref][2]). In other platforms you may be able to send the correct SIGINT to your Spring Boot application. 

Compile to `stop.exe`

# Procrun #

I'm using a script to interact with the service, but you can easily use the command line options.

The relevant parameters are:

```
rem Startup setup
set "PR_STARTUP=auto"
set "PR_STARTMODE=exe"
set "PR_STARTIMAGE=%PATH_TO%\start.exe"
set "PR_STARTPATH=%YOUR_PATH%"
set "PR_STARTPARAMS=%PATH_TO%\my-application.jar#--spring.profiles.active=...#--server.port=9094"

rem Shutdown setup
set "PR_STOPMODE=exe"
set "PR_STOPIMAGE=%PATH_TO%\stop.exe"
set "PR_STOPPATH=%YOUR_PATH%"
set "PR_STOPPARAMS=%PATH_TO%\shutdown.pid"
```

Note that the `StartParams` include the path to the Spring Boot jar and any other options needed separated by `#`. This is why the C application passes all parameters to the `java -jar` command.

# Pros/Cons of this whole mess #

** Pros **

 - The changes to your spring application are minimal, just to use the PID writer.
 - The executables are actually very generic since they don't rely on anything specific, so they can be used with many applications in your deployment servers.

** Cons **

 - You must precompile the start/stop and deploy them to your deployment server for this to work. Depending on your CI/CD or deployment setup this may or may not be easy to achieve.
 - You need to guarantee write permissions on the folder where the jar is located so the PID file can be written by Spring Boot.

  [1]: https://www.baeldung.com/spring-boot-shutdown#bash_script
  [2]: https://stackoverflow.com/a/59898669/5749547

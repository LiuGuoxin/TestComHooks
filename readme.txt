Building and running the project
================================
1) Try building CppCom.  This will fail because CsCom.tlb does not exist but will succeed in generating the CppCom.tlb.
2) Follow the instructions in CppCom readme.txt to generate the Interop.CppCom.dll.
3) Build CsCom (this depends on the Interop.CppCom.dll).
4) Follow the instructions in CsCom readme.txt to generate the CsCom.tlb.
5) Build CppCpm.  This will now succeed.
6) Build Client (this depends on both CppCom.tlb and CsCom.tlb).
7) Run collect.cmd to gather remaining artifacts to the solution Debug directory.
8) Follow the instructions in Client readme.txt to run the different test scenarios.

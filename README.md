# leidenfrost
Local privilege escalation on Windows by abusing CMSTP to bypass User Access Control (UAC)

# How it works

It abuses CMSTP (Connection Manager Profile installer) to run a malicious, user controlled, executable completly bypassing the UAC (User Access Control).

By dropping a crafted .ini file and then installing it using cmstp.exe the post-install executable will be launched at administrator privileges.

# Credits

I did not discover this issue nor I take any credits for it.

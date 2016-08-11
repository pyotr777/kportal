# AWS EC2 CLI Docker image

## Commands

The following shortcut commands are available.

* awsassignip

   Assign elastic (fixed) IP address to EC2 instance.
   Usage: `awsassignip <instance id or tag Name> <Elastic IP address>`.

* awshelp

   List available shortcut commands.

* awsip

   Show assigned IP address of EC2 instance.
   Usage: `$(basename $0) <instance id or tag Name>`.

* awsipls

   List Elastic IP addresses.

* awsls

   List instances IDs, tag "Name" values and instance states (running, stopped, pending,...).
   With -s option prints only instance IDs and tag "Name" value.

* awsrun

   Create and launch a new instance.
   Usage: `awsrun <launch parameters json file> [<tag>]`.
   JSON file must be created beforehand with `aws ec2 run-instances --generate-cli-skeleton ...` command. For details refer to http://docs.aws.amazon.com/cli/latest/reference/ec2/run-instances.html.
   If <tag> argument given, the new instance will be tagged.

* awsstart

   Start stopped instance.
   Usage: `awsstart <instance id or tag Name>`.

* awsstop

   Stop running instance.
   Usage: `awsstop <instance id or tag Name>`.

* awstag

   Assign instance a tag with name "Name".
   Usage: `awstag <instance ID> <tag value>`.

* awsterminate

   Terminate (delete) one or more instances.
   Usage: `awsterminate <instance id or tag Name> [<instance id or tag Name> ...]`

### Note

Most shortcut commands accept tags as well as EC2 instance IDs as arguments, but only tags with name "Name". EC2 tags have names and values. For details refer to http://docs.aws.amazon.com/AWSEC2/latest/UserGuide/Using_Tags.html.
To give EC2 instance a tag "Name" use: `awstag <ID> <tag>`.



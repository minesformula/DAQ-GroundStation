# Live Telemetry

## Installation

Navigate to the "Dist" directory. Select a .whl package version then install the package 
using `pip install <.whl_file>`. This will install the mflivetelemetry cli tool onto
your machine. You may have to ensure that the python installation directory is on your
`$PATH`.

## Usage

mflivetelemetry has two subcommands, configure and live. Before you run the live mode
for the first time you should run `mflivetelemetry configure <API_TOKEN>` where the
`API_TOKEN` is your InfluxDB API token. This will create a hidden file in your home 
directory to save the API token so it shouldn't need to be configured again. Alternatively 
you may use the -k like `mflivetelemetry live -k <API_TOKEN>` flag of the live subcommand 
however your token will not save and you will have to enter it for every consecutive use.
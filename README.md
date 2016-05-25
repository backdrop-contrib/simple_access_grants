# Simple Access Grants

The Simple Access Grants module provides an easy way to control
access to nodes using the Node Access Grants method provided
by Backdrop core.

This module implements a single realm within that method and enables
User Grants to be specified for each user role in combination with
each content type.

User Grants may be applied individually to each node or to every
node of that content type.

User Grants can be edited, added or removed and then reapplied to
all nodes.

A User Grant Profile can be defined that combines multiple grants
for application to a node or all nodes of that type.


## Development

This is a module developed for use with Backdrop CMS and is not a
direct port of a Drupal module. However, it has its origins in a
port of Simple Access, aiming to be a simplification of that
module and to use Backdrop's configuration system.

The aim has been to develop an access grants module that makes full use
of Backdrop's configuration system rather than using tables in the
MySQL database, and also is much simplified by only using the Node
Access Grants method and a single realm.

## Status

This is a working version of the module. Please report any issues
you may find.


## Installation

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
  
  
## Configuration

- No initial configuration is needed.

- Each node and each content type has a new setting 'Access Settings' which,
  if used, will restrict access to that content, either for an individual node
  or for all nodes of that type.

- Additionally, at admin/config/content/simple_access_grants/profiles
  profiles can be added that enable grant settings for more than one role
  to be defined and applied to a selected content type.


## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.
    
        
## Development for Backdrop

Graham Oliver (github.com/Graham-72/)



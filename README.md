# Simple Access Grants

The Simple Access Grants module provides an easy way to control
access to nodes using the Node Access Grants method provided
by Backdrop core.

This module implements a single realm within that method and enables
User Grants to be specified for each user role in combination with
each content type. A new 'access settings' property of each content
type can be configured to grant appropriate access for each user role.

User Grants can be edited, added or removed and then reapplied to
all nodes by rebuilding Backdrop's node_access data-table.

## Status

This new release (1.x-1.1.0) of the project is a further 
simplification and removes the facility to set access grants 
for single nodes. It also removes the complication of setting 
profiles that provide combinations of access settings. 
In practice it would seem adequate to allow for access grants
to be set solely for node/content types.

This is a module developed for use with Backdrop CMS and is not a
direct port of a Drupal module. However, it has its origins in a
port of Simple Access, aiming to be a simplification of that
module and to use Backdrop's configuration system.

The aim has been to develop an access grants module that makes full use
of Backdrop's configuration system rather than using tables in the
MySQL database, and also is much simplified by only using the Node
Access Grants method and a single realm.

## Installation

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.
  
  
## Configuration

- No initial configuration is needed.

- Each content type has a new setting 'Access Settings' which,
  if used, will restrict access to that content for all nodes of that type.
  
- If only certain nodes need restricted access then it is suggested
  that a new type of node is defined e.g. page_restricted, and access
  settings defined for this type of node.
  
- Applying any access setting requires the rebuilding of permissions.
  To provide easier access to this core feature the module adds a
  'Rebuild permissions' item to the admin 'Contents' menu.
  

## License

This project is GPL v2 software. See the LICENSE.txt file in this
directory for complete text.
    
        
## Development for Backdrop

Graham Oliver (github.com/Graham-72/)



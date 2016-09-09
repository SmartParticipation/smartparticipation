Creating a RegRoom custom entity
================================

The RegRoom Sample entity serves as a model. See its associated files:
entities/sample.inc
install/entities/sample.inc

All paths are relative to the regroom module directory.


Schema
------

* Add a file to the schema directory with a function defining the database
table. In the case of the sample entity, the file is named sample.schema.inc.
* Add a require_once() of this file to regroom_schema() in regroom.install.
* Add a call to the function to regroom_schema() in regroom.install.


Entity
------

* Add a file to entities modelled on entities/sample.inc.
* Add calls to functions defined in this file to the following hooks:
    regroom_entity_info()
    regroom_menu()
    regroom_permission()
    regroom_extra_fields()
* Add a require_once() of the file in regroom.module.

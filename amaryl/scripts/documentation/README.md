# The database
We have to modify only one table, *new_v4_users_probes_edit*.

* `ALTER TABLE new_v4_users_probes_edit ADD phone VARCHAR(255);` 
* `ALTER TABLE new_v4_users_probes_edit ADD email VARCHAR(255);`
* `ALTER TABLE new_v4_users_probes_edit ADD ref tinyint(1);`

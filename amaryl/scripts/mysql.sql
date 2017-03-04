ALTER TABLE new_v4_users_probes_edit ADD phone VARCHAR(255);
ALTER TABLE new_v4_users_probes_edit ADD email VARCHAR(255);
ALTER TABLE new_v4_users_probes_edit ADD ref tinyint(1);
ALTER TABLE new_v4_sonde ADD firsttime int(11) unsigned after uptime;

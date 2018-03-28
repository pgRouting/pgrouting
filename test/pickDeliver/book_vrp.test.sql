DROP TABLE IF EXISTS jet_customers;


/*
CREATE TABLE trucks(id integer primary key, truck_name varchar(75), capacity integer );
INSERT INTO trucks(id, truck_name, capacity)
VALUES (1, 'Hardy Truck', 800),
    (2, 'Laurel Truck', 400);

CREATE TABLE restaurants_99(id integer primary key,  loc_name varchar(150), lon float, lat float);
INSERT INTO restaurants_99(id, loc_name, lon, lat)
VALUES (1, 'Bristol, CT', -72.8924, 41.66892),
     (2, 'Cromwell, CT', -72.67615, 41.60165),
     (3, 'Danbury, CT', -73.41376, 41.41008),
     (4, 'Enfield, CT', -72.57634, 41.98837),
     (5, 'Glastonbury, CT', -72.61592, 41.72646),
     (6, 'Groton, CT', -72.06683, 41.36298),
     (7, 'Dayville, CT', -71.8824882, 41.8425568),
     (8, 'Norwich, CT', -72.10508, 41.51431),
     (9, 'Stratford, CT',  -73.12182, 41.19878),
     (10, 'Torrington, CT',  -73.12131, 41.79984),
     (11, 'Vernon, CT', -72.49305, 41.82647),
     (12, 'Wallingford, CT', -72.8092, 41.4835);
*/

CREATE TABLE jet_customers (
    airport character varying,
    id integer,
    x double precision,
    y double precision,
    pindex integer,
    dindex integer,
    demand integer,
    opentime double precision,
    closetime double precision,
    servicetime double precision
);


/*
--
-- TOC entry 290 (class 1259 OID 6616043)
-- Name: vrp1_cost; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE vrp1_cost (
    src_id integer,
    dest_id integer,
    cost double precision,
    distance double precision,
    traveltime double precision
);



--
-- TOC entry 291 (class 1259 OID 6616046)
-- Name: vrp1_gdist; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE vrp1_gdist (
    src_id integer,
    dest_id integer,
    cost double precision,
    distance double precision,
    traveltime double precision
);


--
-- TOC entry 285 (class 1259 OID 6607823)
-- Name: vrp1_orders; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE vrp1_orders (
    id integer,
    x numeric,
    y numeric,
    order_unit integer,
    open_time integer,
    close_time integer,
    service_time integer
);



--
-- TOC entry 289 (class 1259 OID 6616034)
-- Name: vrp1_points; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE vrp1_points (
    id integer,
    pid integer,
    geom geometry(Point,4326),
    edge_id integer,
    fraction double precision,
    closest_node integer
);


--
-- TOC entry 297 (class 1259 OID 8777648)
-- Name: vrpdtw_beer; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE vrpdtw_beer (
    rid integer,
    id integer,
    x double precision,
    y double precision,
    opentime integer,
    closetime integer,
    servicetime integer,
    demand integer,
    pindex integer,
    dindex integer
);
*/

--

INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 1, 2320738.4688434978, 126348.38967116659, 0, 2, 4, 3448, 2853448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 3, 2320738.4688434978, 126348.38967116659, 0, 4, 4, 3448, 2853448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 5, 2320738.4688434978, 126348.38967116659, 0, 6, 4, 3448, 2853448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 7, 2320738.4688434978, 126348.38967116659, 0, 8, 4, 3448, 2853448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 9, 2320738.4688434978, 126348.38967116659, 0, 10, 4, 3448, 2853448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 11, 2320738.4688434978, 126348.38967116659, 0, 12, 4, 3448, 2853448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 13, 2383186.1525343237, 448251.25888828852, 0, 14, 4, 3463, 2853463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 15, 2383186.1525343237, 448251.25888828852, 0, 16, 4, 3463, 2853463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 17, 2383186.1525343237, 448251.25888828852, 0, 18, 4, 3463, 2853463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 19, 2383186.1525343237, 448251.25888828852, 0, 20, 4, 3463, 2853463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 21, 2383186.1525343237, 448251.25888828852, 0, 22, 4, 3463, 2853463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 23, 2383186.1525343237, 448251.25888828852, 0, 24, 4, 3463, 2853463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 25, 1981144.5525029898, -1903041.2384292425, 0, 26, 2, 3576, 2853576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 27, 1981144.5525029898, -1903041.2384292425, 0, 28, 2, 3576, 2853576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 29, 1981144.5525029898, -1903041.2384292425, 0, 30, 2, 3576, 2853576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 31, 1981144.5525029898, -1903041.2384292425, 0, 32, 2, 3576, 2853576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 33, 1981144.5525029898, -1903041.2384292425, 0, 34, 2, 3576, 2853576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 35, 1981144.5525029898, -1903041.2384292425, 0, 36, 2, 3576, 2853576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 37, 2138409.5568088419, -119451.50568778868, 0, 38, 5, 3624, 2853624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 39, 2138409.5568088419, -119451.50568778868, 0, 40, 5, 3624, 2853624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 41, 2138409.5568088419, -119451.50568778868, 0, 42, 5, 3624, 2853624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 43, 2138409.5568088419, -119451.50568778868, 0, 44, 5, 3624, 2853624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 45, 2138409.5568088419, -119451.50568778868, 0, 46, 5, 3624, 2853624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 47, 2138409.5568088419, -119451.50568778868, 0, 48, 5, 3624, 2853624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 49, 2387491.1007072991, 34459.351848672632, 0, 50, 1, 4265, 2854265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 51, 2387491.1007072991, 34459.351848672632, 0, 52, 1, 4265, 2854265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 53, 2387491.1007072991, 34459.351848672632, 0, 54, 1, 4265, 2854265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 55, 2387491.1007072991, 34459.351848672632, 0, 56, 1, 4265, 2854265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 57, 2387491.1007072991, 34459.351848672632, 0, 58, 1, 4265, 2854265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 59, 2387491.1007072991, 34459.351848672632, 0, 60, 1, 4265, 2854265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 61, 2035310.7411768832, -176076.78362264115, 0, 62, 1, 4355, 2854355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 63, 2035310.7411768832, -176076.78362264115, 0, 64, 1, 4355, 2854355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 65, 2035310.7411768832, -176076.78362264115, 0, 66, 1, 4355, 2854355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 67, 2035310.7411768832, -176076.78362264115, 0, 68, 1, 4355, 2854355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 69, 2035310.7411768832, -176076.78362264115, 0, 70, 1, 4355, 2854355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 71, 2035310.7411768832, -176076.78362264115, 0, 72, 1, 4355, 2854355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 2, 2320738.4688434978, 126348.38967116659, 1, 0, -4, 3448, 6903448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 4, 2383186.1525343237, 448251.25888828852, 3, 0, -4, 3448, 6903448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 6, 1981144.5525029898, -1903041.2384292425, 5, 0, -4, 3448, 6903448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 8, 2138409.5568088419, -119451.50568778868, 7, 0, -4, 3448, 6903448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 10, 2387491.1007072991, 34459.351848672632, 9, 0, -4, 3448, 6903448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 12, 2035310.7411768832, -176076.78362264115, 11, 0, -4, 3448, 6903448, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 14, 2320738.4688434978, 126348.38967116659, 13, 0, -4, 3463, 6903463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 16, 2383186.1525343237, 448251.25888828852, 15, 0, -4, 3463, 6903463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 18, 1981144.5525029898, -1903041.2384292425, 17, 0, -4, 3463, 6903463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 20, 2138409.5568088419, -119451.50568778868, 19, 0, -4, 3463, 6903463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 22, 2387491.1007072991, 34459.351848672632, 21, 0, -4, 3463, 6903463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 24, 2035310.7411768832, -176076.78362264115, 23, 0, -4, 3463, 6903463, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 26, 2320738.4688434978, 126348.38967116659, 25, 0, -2, 3576, 6903576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 28, 2383186.1525343237, 448251.25888828852, 27, 0, -2, 3576, 6903576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 30, 1981144.5525029898, -1903041.2384292425, 29, 0, -2, 3576, 6903576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 32, 2138409.5568088419, -119451.50568778868, 31, 0, -2, 3576, 6903576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 34, 2387491.1007072991, 34459.351848672632, 33, 0, -2, 3576, 6903576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 36, 2035310.7411768832, -176076.78362264115, 35, 0, -2, 3576, 6903576, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 38, 2320738.4688434978, 126348.38967116659, 37, 0, -5, 3624, 6903624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 40, 2383186.1525343237, 448251.25888828852, 39, 0, -5, 3624, 6903624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 42, 1981144.5525029898, -1903041.2384292425, 41, 0, -5, 3624, 6903624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 44, 2138409.5568088419, -119451.50568778868, 43, 0, -5, 3624, 6903624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 46, 2387491.1007072991, 34459.351848672632, 45, 0, -5, 3624, 6903624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 48, 2035310.7411768832, -176076.78362264115, 47, 0, -5, 3624, 6903624, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 50, 2320738.4688434978, 126348.38967116659, 49, 0, -1, 4265, 6904265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 52, 2383186.1525343237, 448251.25888828852, 51, 0, -1, 4265, 6904265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 54, 1981144.5525029898, -1903041.2384292425, 53, 0, -1, 4265, 6904265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 56, 2138409.5568088419, -119451.50568778868, 55, 0, -1, 4265, 6904265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 58, 2387491.1007072991, 34459.351848672632, 57, 0, -1, 4265, 6904265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 60, 2035310.7411768832, -176076.78362264115, 59, 0, -1, 4265, 6904265, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BOS', 62, 2320738.4688434978, 126348.38967116659, 61, 0, -1, 4355, 6904355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('BGR', 64, 2383186.1525343237, 448251.25888828852, 63, 0, -1, 4355, 6904355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MIA', 66, 1981144.5525029898, -1903041.2384292425, 65, 0, -1, 4355, 6904355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 68, 2138409.5568088419, -119451.50568778868, 67, 0, -1, 4355, 6904355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('MVY', 70, 2387491.1007072991, 34459.351848672632, 69, 0, -1, 4355, 6904355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('ABE', 72, 2035310.7411768832, -176076.78362264115, 71, 0, -1, 4355, 6904355, 450000);
INSERT INTO jet_customers (airport, id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) VALUES ('TEB', 0, 2138409.5568088419, -119451.50568778868, 0, 0, 0, 0, 7200000, 0);

/*
--
-- TOC entry 4292 (class 0 OID 6616043)
-- Dependencies: 290
-- Data for Name: vrp1_cost; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 13, 2953.093354567417, 60557.741467469998, 49.218222576123615);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 13, 2951.4358855784913, 46704.633074669997, 49.190598092974852);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 13, 1933.2635307127596, 29674.281907879998, 32.22105884521266);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 13, 4891.243355445933, 78590.388493249993, 81.520722590765544);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 13, 4564.0248344551255, 55146.744125789999, 76.067080574252088);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 13, 358.9916907880604, 4717.2985756400003, 5.983194846467673);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 13, 5126.2324354788034, 62516.944979489999, 85.437207257980063);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 13, 6000.6726547073304, 97970.007298349999, 100.01121091178884);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 13, 4755.4348034907207, 100603.89591172, 79.257246724845345);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 13, 4138.1137957014362, 53942.09578946, 68.968563261690605);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 13, 2224.3765052979202, 44388.04507498, 37.072941754965335);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 13, 1469.4570414749417, 17611.434113070001, 24.490950691249029);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 12, 6552.1443518080496, 86875.647316550007, 109.20240586346749);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 12, 3675.7683635329909, 63485.069269129999, 61.262806058883179);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 12, 1826.0827112688785, 21731.213745190002, 30.434711854481307);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 12, 2681.3122972236029, 46309.024619509997, 44.688538287060048);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 12, 1020.3947307304589, 17188.5257222, 17.006578845507651);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 12, 2827.6100244902282, 43710.459317699999, 47.126833741503802);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 12, 3888.0148463653441, 51173.467957710003, 64.800247439422407);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 12, 2302.9311138070507, 41043.449335719997, 38.382185230117514);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 12, 4463.5497447224634, 58891.64477087, 74.39249574537439);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 12, 2647.2156693505872, 59326.863049970001, 44.120261155843117);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 12, 2983.3734066886564, 60557.741467469998, 49.722890111477604);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 12, 1517.9735472205105, 31428.651624779999, 25.29955912034184);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 11, 2745.3112518799626, 37544.1375137, 45.755187531332709);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 11, 4471.1319444284782, 87264.291763329995, 74.518865740474638);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 11, 4229.3311896072573, 52287.419420220001, 70.488853160120954);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 11, 4506.8776151690954, 62551.773221880001, 75.11462691948492);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 11, 5716.3697956450824, 89593.452941569994, 95.272829927418044);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 11, 1358.6096137713382, 15091.14100876, 22.643493562855635);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 11, 1654.544693329585, 19264.633366329999, 27.575744888826417);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 11, 2668.7904955051708, 46309.024619509997, 44.479841591752844);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 11, 1940.0736462356747, 29252.344344699999, 32.334560770594578);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 11, 1469.4570414749414, 17611.434113070001, 24.490950691249022);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 11, 4064.8414892363171, 50747.73846031, 67.74735815393862);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 11, 3895.6963212041123, 47391.047207520001, 64.928272020068533);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 10, 3368.9527734188123, 42809.778697560003, 56.149212890313535);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 10, 8182.3826173823836, 103032.57156053001, 136.37304362303973);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 10, 4258.7560110340182, 52287.419420220001, 70.979266850566972);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 10, 2839.6114198410569, 43099.30820806, 47.326856997350951);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 10, 1802.3572076471451, 23960.724915679999, 30.039286794119086);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 10, 3599.4301883137537, 49681.572728439998, 59.990503138562559);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 10, 4341.7812292867338, 66755.884535439996, 72.363020488112227);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 10, 3856.5004779654537, 49838.427136099999, 64.275007966090897);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 10, 6294.2596511785378, 90390.880059980002, 104.90432751964229);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 10, 5506.4782699890657, 100431.42119920001, 91.77463783315109);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 10, 2855.7053715208308, 43710.459317699999, 47.595089525347177);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 10, 4192.6582153035215, 53942.09578946, 69.877636921725355);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 9, 4325.4261283707883, 66755.884535439996, 72.090435472846465);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 9, 3398.8938935332781, 72255.826813129999, 56.648231558887964);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 9, 2901.3150770432248, 58231.554459619998, 48.35525128405375);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 9, 4528.1360156633564, 98787.207065139999, 75.46893359438927);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 9, 3788.1003008742573, 55624.35354348, 63.135005014570957);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 9, 4562.232643536373, 87264.291763329995, 76.037210725606215);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 9, 4821.657825336647, 92014.350547320006, 80.360963755610783);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 9, 8117.448634400117, 125749.52752643, 135.29081057333528);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 9, 4864.293753001426, 100603.89591172, 81.071562550023771);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 9, 4033.8764441471735, 90244.940354680002, 67.231274069119564);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 9, 2328.9720943505417, 41043.449335719997, 38.81620157250903);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 9, 2403.8510779993576, 33888.802388440003, 40.064184633322625);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 8, 6769.2775240040783, 109943.3259573, 112.82129206673464);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 8, 5019.7492752187654, 67853.964925620006, 83.662487920312756);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 8, 3295.7908090634696, 40898.252999240001, 54.929846817724496);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 8, 3571.0503376979805, 48617.072219770002, 59.517505628299674);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 8, 4729.9553726845033, 92014.350547320006, 78.832589544741722);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 8, 5123.9825769607578, 62516.944979489999, 85.399709616012629);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 8, 1419.3370493081236, 17108.384354760001, 23.655617488468728);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 8, 4462.5946410243814, 58891.64477087, 74.37657735040635);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 8, 3906.5702317981809, 48659.42172359, 65.109503863303019);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 8, 5406.1709302501031, 65640.18822928, 90.102848837501725);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 8, 6449.5678543462845, 90390.880059980002, 107.49279757243808);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 8, 3903.2468943918611, 47391.047207520001, 65.054114906531012);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 7, 6541.7293294122628, 86875.647316550007, 109.02882215687104);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 7, 9589.308629552168, 136345.04018709, 159.82181049253614);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 7, 8189.9389451185689, 103032.57156053001, 136.49898241864281);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 7, 3295.7035800954918, 40898.252999240001, 54.92839300159153);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 7, 4082.2196582153956, 50747.73846031, 68.036994303589921);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 7, 5106.1634678476485, 62318.437049, 85.102724464127476);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 7, 4426.6022183180712, 55437.00628994, 73.776703638634515);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 7, 5813.0124801427592, 71255.008476129995, 96.88354133571265);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 7, 8025.6589527799933, 125749.52752643, 133.76098254633322);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 7, 4565.7908933697781, 55146.744125789999, 76.096514889496305);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 7, 6646.402085744041, 86176.316629309993, 110.77336809573401);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 7, 4924.7825841578369, 59797.006035819999, 82.079709735963945);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 6, 4433.5593311775692, 55437.00628994, 73.89265551962616);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 6, 4943.220062989687, 78590.388493249993, 82.387001049828115);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 6, 3663.4860364258975, 63485.069269129999, 61.058100607098289);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 6, 4496.7745274249164, 62551.773221880001, 74.946242123748604);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 6, 5650.4592497478006, 100431.42119920001, 94.174320829130011);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 6, 3930.8467680860199, 90244.940354680002, 65.514112801433669);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 6, 1427.2238809627568, 17108.384354760001, 23.787064682712614);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 6, 3477.8202035215431, 61063.323590389999, 57.963670058692387);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 6, 4607.0623256516183, 81392.253960899994, 76.784372094193643);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 6, 5970.1689194055944, 112779.81503285001, 99.502815323426574);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 6, 2771.9417330994952, 57379.320287560004, 46.199028884991584);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 6, 4220.6406706202815, 76835.995936859996, 70.344011177004688);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 5, 1958.2616125989289, 23885.506073140001, 32.637693543315478);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 5, 1532.4187981313769, 31428.651624779999, 25.540313302189613);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 5, 3929.4957334296996, 48659.42172359, 65.491595557161659);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 5, 1958.1139397295849, 29674.281907879998, 32.635232328826412);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 5, 5107.3400430688225, 62318.437049, 85.122334051147035);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 5, 3519.5932763961732, 42809.778697560003, 58.659887939936219);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 5, 3334.7602470546817, 72255.826813129999, 55.57933745091136);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 5, 1621.9562023915164, 29275.540332420002, 27.03260337319194);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 5, 803.70194886187926, 14741.93432463, 13.39503248103132);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 5, 4579.9980982712887, 75253.614561519993, 76.333301637854817);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 5, 1337.554729509015, 15091.14100876, 22.292578825150251);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 5, 3470.5687990098932, 61063.323590389999, 57.842813316831553);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 4, 2616.215006867988, 59326.863049970001, 43.603583447799799);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 4, 1655.8289719890095, 19264.633366329999, 27.597149533150159);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 4, 1887.4981575984907, 43746.497693609999, 31.458302626641512);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 4, 4923.0165252431843, 59797.006035819999, 82.050275420719743);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 4, 5342.146388935982, 65640.18822928, 89.035773148933032);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 4, 4418.556455791293, 98787.207065139999, 73.642607596521557);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 4, 358.9916907880604, 4717.2985756400003, 5.983194846467673);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 4, 3801.2354480020076, 49838.427136099999, 63.353924133366796);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 4, 2614.5575378790622, 44139.442491629998, 43.575958964651036);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 4, 5663.7943070079027, 94785.168599109995, 94.396571783465049);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 4, 4554.3650077465054, 81392.253960899994, 75.906083462441757);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 4, 1596.3851830133301, 29275.540332420002, 26.606419716888833);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 3, 4354.0799478841718, 51173.467957710003, 72.567999131402857);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 3, 6464.3892097938715, 97970.007298349999, 107.73982016323119);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 3, 10033.160154913267, 136345.04018709, 167.21933591522111);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 3, 4016.3136218536224, 49681.572728439998, 66.938560364227044);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 3, 6162.3281003288175, 89593.452941569994, 102.70546833881363);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 3, 2746.7160740841282, 33888.802388440003, 45.778601234735468);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 3, 4998.9893503257272, 75253.614561519993, 83.316489172095459);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 3, 7276.4598265076593, 109943.3259573, 121.27433044179432);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 3, 4325.3148995953306, 52145.170245989997, 72.088581659922184);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 3, 6128.2314724558018, 94785.168599109995, 102.1371912075967);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 3, 6488.6784453181817, 112779.81503285001, 108.14464075530303);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 3, 4501.4105338356794, 65153.684267479999, 75.023508897261323);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (1, 2, 1536.6099447776205, 19506.03254122, 25.610165746293674);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 2, 5827.9602822664219, 71255.008476129995, 97.132671371107037);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 2, 3571.4879476634851, 48617.072219770002, 59.524799127724755);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 2, 793.78947767888133, 14741.93432463, 13.229824627981355);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 2, 2787.0688014921589, 58231.554459619998, 46.451146691535982);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 2, 1923.0315998089593, 43746.497693609999, 32.050526663482657);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 2, 2971.9018308336504, 43099.30820806, 49.53169718056084);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 2, 4032.3066527087663, 65153.684267479999, 67.205110878479431);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 2, 1957.1282276819745, 29252.344344699999, 32.618803794699573);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 2, 984.72735256885392, 17188.5257222, 16.412122542814231);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 2, 2259.1893371470278, 44388.04507498, 37.653155619117129);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 2, 2783.7065664740117, 57379.320287560004, 46.395109441233529);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (3, 1, 3909.9615503187342, 52145.170245989997, 65.166025838645567);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (11, 1, 2796.4128490759304, 37544.1375137, 46.606880817932172);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (13, 1, 3063.0403515469643, 46704.633074669997, 51.050672525782737);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (6, 1, 3770.599595643097, 76835.995936859996, 62.843326594051618);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (10, 1, 1806.0013795626483, 23960.724915679999, 30.100022992710805);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (4, 1, 2726.8826142088951, 44139.442491629998, 45.448043570148251);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (9, 1, 3680.3813452318182, 55624.35354348, 61.339689087196966);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (5, 1, 1633.0740990728355, 23885.506073140001, 27.217901651213925);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (7, 1, 6667.2449036603712, 86176.316629309993, 111.12074839433951);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (8, 1, 4558.3809768325664, 67853.964925620006, 75.97301628054278);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (12, 1, 1833.1811824131848, 21731.213745190002, 30.553019706886413);
INSERT INTO vrp1_cost (src_id, dest_id, cost, distance, traveltime) VALUES (2, 1, 1103.7327454950807, 19506.03254122, 18.395545758251345);


--
-- TOC entry 4293 (class 0 OID 6616046)
-- Dependencies: 291
-- Data for Name: vrp1_gdist; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 2, 19506.03254122, 19506.03254122, 0.0065020108470733333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 3, 52145.170245989997, 52145.170245989997, 0.017381723415329998);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 4, 44139.442491629998, 44139.442491629998, 0.01471314749721);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 5, 23885.506073140001, 23885.506073140001, 0.0079618353577133334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 6, 76835.995936859996, 76835.995936859996, 0.025611998645620002);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 7, 86176.316629309993, 86176.316629309993, 0.028725438876436662);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 8, 67853.964925620006, 67853.964925620006, 0.022617988308540001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 9, 55624.35354348, 55624.35354348, 0.018541451181160001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 10, 23960.724915679999, 23960.724915679999, 0.0079869083052266678);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 11, 37544.1375137, 37544.1375137, 0.012514712504566667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 12, 21731.213745190002, 21731.213745190002, 0.0072437379150633334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (1, 13, 46704.633074669997, 46704.633074669997, 0.01556821102489);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 1, 19506.03254122, 19506.03254122, 0.0065020108470733333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 3, 65153.684267479999, 65153.684267479999, 0.02171789475582667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 4, 43746.497693609999, 43746.497693609999, 0.01458216589787);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 5, 14741.93432463, 14741.93432463, 0.0049139781082099994);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 6, 57379.320287560004, 57379.320287560004, 0.019126440095853337);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 7, 71255.008476129995, 71255.008476129995, 0.023751669492043333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 8, 48617.072219770002, 48617.072219770002, 0.016205690739923333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 9, 58231.554459619998, 58231.554459619998, 0.019410518153206668);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 10, 43099.30820806, 43099.30820806, 0.014366436069353334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 11, 29252.344344699999, 29252.344344699999, 0.0097507814482333324);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 12, 17188.5257222, 17188.5257222, 0.0057295085740666669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (2, 13, 44388.04507498, 44388.04507498, 0.014796015024993333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 1, 52145.170245989997, 52145.170245989997, 0.017381723415329998);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 2, 65153.684267479999, 65153.684267479999, 0.02171789475582667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 4, 94785.168599109995, 94785.168599109995, 0.031595056199703334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 5, 75253.614561519993, 75253.614561519993, 0.025084538187173332);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 6, 112779.81503285001, 112779.81503285001, 0.037593271677616676);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 7, 136345.04018709, 136345.04018709, 0.045448346729030004);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 8, 109943.3259573, 109943.3259573, 0.036647775319099997);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 9, 33888.802388440003, 33888.802388440003, 0.011296267462813335);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 10, 49681.572728439998, 49681.572728439998, 0.016560524242813332);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 11, 89593.452941569994, 89593.452941569994, 0.029864484313856667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 12, 51173.467957710003, 51173.467957710003, 0.017057822652570002);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (3, 13, 97970.007298349999, 97970.007298349999, 0.032656669099449996);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 1, 44139.442491629998, 44139.442491629998, 0.01471314749721);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 2, 43746.497693609999, 43746.497693609999, 0.01458216589787);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 3, 94785.168599109995, 94785.168599109995, 0.031595056199703334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 5, 29275.540332420002, 29275.540332420002, 0.0097585134441400007);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 6, 81392.253960899994, 81392.253960899994, 0.0271307513203);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 7, 59797.006035819999, 59797.006035819999, 0.019932335345273333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 8, 65640.18822928, 65640.18822928, 0.021880062743093332);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 9, 98787.207065139999, 98787.207065139999, 0.032929069021713331);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 10, 49838.427136099999, 49838.427136099999, 0.016612809045366668);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 11, 19264.633366329999, 19264.633366329999, 0.0064215444554433336);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 12, 59326.863049970001, 59326.863049970001, 0.019775621016656666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (4, 13, 4717.2985756400003, 4717.2985756400003, 0.0015724328585466666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 1, 23885.506073140001, 23885.506073140001, 0.0079618353577133334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 2, 14741.93432463, 14741.93432463, 0.0049139781082099994);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 3, 75253.614561519993, 75253.614561519993, 0.025084538187173332);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 4, 29275.540332420002, 29275.540332420002, 0.0097585134441400007);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 6, 61063.323590389999, 61063.323590389999, 0.020354441196796667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 7, 62318.437049, 62318.437049, 0.020772812349666669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 8, 48659.42172359, 48659.42172359, 0.016219807241196666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 9, 72255.826813129999, 72255.826813129999, 0.024085275604376667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 10, 42809.778697560003, 42809.778697560003, 0.014269926232520002);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 11, 15091.14100876, 15091.14100876, 0.0050303803362533334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 12, 31428.651624779999, 31428.651624779999, 0.010476217208259999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (5, 13, 29674.281907879998, 29674.281907879998, 0.0098914273026266646);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 1, 76835.995936859996, 76835.995936859996, 0.025611998645620002);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 2, 57379.320287560004, 57379.320287560004, 0.019126440095853337);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 3, 112779.81503285001, 112779.81503285001, 0.037593271677616676);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 4, 81392.253960899994, 81392.253960899994, 0.0271307513203);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 5, 61063.323590389999, 61063.323590389999, 0.020354441196796667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 7, 55437.00628994, 55437.00628994, 0.018479002096646668);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 8, 17108.384354760001, 17108.384354760001, 0.0057027947849200003);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 9, 90244.940354680002, 90244.940354680002, 0.030081646784893331);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 10, 100431.42119920001, 100431.42119920001, 0.033477140399733339);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 11, 62551.773221880001, 62551.773221880001, 0.020850591073959999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 12, 63485.069269129999, 63485.069269129999, 0.021161689756376667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (6, 13, 78590.388493249993, 78590.388493249993, 0.026196796164416662);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 1, 86176.316629309993, 86176.316629309993, 0.028725438876436662);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 2, 71255.008476129995, 71255.008476129995, 0.023751669492043333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 3, 136345.04018709, 136345.04018709, 0.045448346729030004);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 4, 59797.006035819999, 59797.006035819999, 0.019932335345273333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 5, 62318.437049, 62318.437049, 0.020772812349666669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 6, 55437.00628994, 55437.00628994, 0.018479002096646668);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 8, 40898.252999240001, 40898.252999240001, 0.013632750999746666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 9, 125749.52752643, 125749.52752643, 0.041916509175476666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 10, 103032.57156053001, 103032.57156053001, 0.034344190520176669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 11, 50747.73846031, 50747.73846031, 0.016915912820103334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 12, 86875.647316550007, 86875.647316550007, 0.028958549105516665);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (7, 13, 55146.744125789999, 55146.744125789999, 0.018382248041930001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 1, 67853.964925620006, 67853.964925620006, 0.022617988308540001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 2, 48617.072219770002, 48617.072219770002, 0.016205690739923333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 3, 109943.3259573, 109943.3259573, 0.036647775319099997);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 4, 65640.18822928, 65640.18822928, 0.021880062743093332);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 5, 48659.42172359, 48659.42172359, 0.016219807241196666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 6, 17108.384354760001, 17108.384354760001, 0.0057027947849200003);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 7, 40898.252999240001, 40898.252999240001, 0.013632750999746666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 9, 92014.350547320006, 92014.350547320006, 0.030671450182440003);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 10, 90390.880059980002, 90390.880059980002, 0.030130293353326669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 11, 47391.047207520001, 47391.047207520001, 0.015797015735840001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 12, 58891.64477087, 58891.64477087, 0.019630548256956665);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (8, 13, 62516.944979489999, 62516.944979489999, 0.020838981659829999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 1, 55624.35354348, 55624.35354348, 0.018541451181160001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 2, 58231.554459619998, 58231.554459619998, 0.019410518153206668);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 3, 33888.802388440003, 33888.802388440003, 0.011296267462813335);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 4, 98787.207065139999, 98787.207065139999, 0.032929069021713331);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 5, 72255.826813129999, 72255.826813129999, 0.024085275604376667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 6, 90244.940354680002, 90244.940354680002, 0.030081646784893331);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 7, 125749.52752643, 125749.52752643, 0.041916509175476666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 8, 92014.350547320006, 92014.350547320006, 0.030671450182440003);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 10, 66755.884535439996, 66755.884535439996, 0.022251961511813335);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 11, 87264.291763329995, 87264.291763329995, 0.029088097254443331);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 12, 41043.449335719997, 41043.449335719997, 0.013681149778573333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (9, 13, 100603.89591172, 100603.89591172, 0.033534631970573339);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 1, 23960.724915679999, 23960.724915679999, 0.0079869083052266678);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 2, 43099.30820806, 43099.30820806, 0.014366436069353334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 3, 49681.572728439998, 49681.572728439998, 0.016560524242813332);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 4, 49838.427136099999, 49838.427136099999, 0.016612809045366668);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 5, 42809.778697560003, 42809.778697560003, 0.014269926232520002);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 6, 100431.42119920001, 100431.42119920001, 0.033477140399733339);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 7, 103032.57156053001, 103032.57156053001, 0.034344190520176669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 8, 90390.880059980002, 90390.880059980002, 0.030130293353326669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 9, 66755.884535439996, 66755.884535439996, 0.022251961511813335);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 11, 52287.419420220001, 52287.419420220001, 0.017429139806740001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 12, 43710.459317699999, 43710.459317699999, 0.014570153105900001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (10, 13, 53942.09578946, 53942.09578946, 0.017980698596486665);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 1, 37544.1375137, 37544.1375137, 0.012514712504566667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 2, 29252.344344699999, 29252.344344699999, 0.0097507814482333324);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 3, 89593.452941569994, 89593.452941569994, 0.029864484313856667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 4, 19264.633366329999, 19264.633366329999, 0.0064215444554433336);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 5, 15091.14100876, 15091.14100876, 0.0050303803362533334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 6, 62551.773221880001, 62551.773221880001, 0.020850591073959999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 7, 50747.73846031, 50747.73846031, 0.016915912820103334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 8, 47391.047207520001, 47391.047207520001, 0.015797015735840001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 9, 87264.291763329995, 87264.291763329995, 0.029088097254443331);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 10, 52287.419420220001, 52287.419420220001, 0.017429139806740001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 12, 46309.024619509997, 46309.024619509997, 0.015436341539836666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (11, 13, 17611.434113070001, 17611.434113070001, 0.0058704780376900006);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 1, 21731.213745190002, 21731.213745190002, 0.0072437379150633334);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 2, 17188.5257222, 17188.5257222, 0.0057295085740666669);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 3, 51173.467957710003, 51173.467957710003, 0.017057822652570002);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 4, 59326.863049970001, 59326.863049970001, 0.019775621016656666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 5, 31428.651624779999, 31428.651624779999, 0.010476217208259999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 6, 63485.069269129999, 63485.069269129999, 0.021161689756376667);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 7, 86875.647316550007, 86875.647316550007, 0.028958549105516665);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 8, 58891.64477087, 58891.64477087, 0.019630548256956665);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 9, 41043.449335719997, 41043.449335719997, 0.013681149778573333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 10, 43710.459317699999, 43710.459317699999, 0.014570153105900001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 11, 46309.024619509997, 46309.024619509997, 0.015436341539836666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (12, 13, 60557.741467469998, 60557.741467469998, 0.020185913822489999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 1, 46704.633074669997, 46704.633074669997, 0.01556821102489);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 2, 44388.04507498, 44388.04507498, 0.014796015024993333);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 3, 97970.007298349999, 97970.007298349999, 0.032656669099449996);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 4, 4717.2985756400003, 4717.2985756400003, 0.0015724328585466666);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 5, 29674.281907879998, 29674.281907879998, 0.0098914273026266646);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 6, 78590.388493249993, 78590.388493249993, 0.026196796164416662);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 7, 55146.744125789999, 55146.744125789999, 0.018382248041930001);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 8, 62516.944979489999, 62516.944979489999, 0.020838981659829999);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 9, 100603.89591172, 100603.89591172, 0.033534631970573339);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 10, 53942.09578946, 53942.09578946, 0.017980698596486665);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 11, 17611.434113070001, 17611.434113070001, 0.0058704780376900006);
INSERT INTO vrp1_gdist (src_id, dest_id, cost, distance, traveltime) VALUES (13, 12, 60557.741467469998, 60557.741467469998, 0.020185913822489999);


--
-- TOC entry 4290 (class 0 OID 6607823)
-- Dependencies: 285
-- Data for Name: vrp1_orders; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (1, -72.892400, 41.668920, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (2, -72.676150, 41.601650, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (3, -73.413760, 41.410080, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (4, -72.576340, 41.988370, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (5, -72.615920, 41.726460, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (6, -72.066830, 41.362980, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (7, -71.882488, 41.842557, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (8, -72.105080, 41.514310, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (9, -73.121820, 41.198780, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (10, -73.121310, 41.799840, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (11, -72.493050, 41.826470, 250, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (12, -72.809200, 41.483500, 25, 60, 780, 20);
INSERT INTO vrp1_orders (id, x, y, order_unit, open_time, close_time, service_time) VALUES (13, -72.51975, 41.98377, 0, 0, 800, 0);


--
-- TOC entry 4291 (class 0 OID 6616034)
-- Dependencies: 289
-- Data for Name: vrp1_points; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (1, -1, '0101000020E6100000D3BCE3141D3952C0F6D1A92B9FD54440', 134983, 0.38479479629572311, 69021);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (2, -2, '0101000020E6100000304CA60A462B52C0B7D100DE02CD4440', 156235, 0.60081527076326913, 174662);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (3, -3, '0101000020E61000002619390B7B5A52C0315F5E807DB44440', 155907, 0.60929293991596534, 127670);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (4, -4, '0101000020E61000001BD82AC1E22452C07C2C7DE882FE4440', 82159, 0.1088234404534253, 56829);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (5, -5, '0101000020E6100000F33CB83B6B2752C014ED2AA4FCDC4440', 226752, 0.87665768706747127, 129246);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (6, -6, '0101000020E6100000111956F1460452C0118DEE2076AE4440', 133685, 0.91338793111615413, 28564);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (7, -7, '0101000020E610000032C7F2AE7AF851C0080264E8D8EB4440', 263400, 0.83072231921053108, 196779);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (8, -8, '0101000020E6100000ADDD76A1B90652C0BD00FBE8D4C14440', 47970, 0.21514413450004088, 25658);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (9, -9, '0101000020E6100000EBFF1CE6CB4752C0A88C7F9F71994440', 30632, 0.85391427066903536, 20036);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (10, -10, '0101000020E610000060AB048BC34752C02E04392861E64440', 231149, 0.50999310422417587, 153595);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (11, -11, '0101000020E6100000BD5296218E1F52C00490DAC4C9E94440', 152917, 0.70955464807036772, 159294);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (12, -12, '0101000020E610000016FBCBEEC93352C0D9CEF753E3BD4440', 235710, 0.44298119784627749, 124069);
INSERT INTO vrp1_points (id, pid, geom, edge_id, fraction, closest_node) VALUES (13, -13, '0101000020E610000025068195432152C09964E42CECFD4440', 220656, 0.77224957554563523, 120633);


--
-- TOC entry 4295 (class 0 OID 8777648)
-- Dependencies: 297
-- Data for Name: vrpdtw_beer; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 0, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 0, 0, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 1, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 2);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 3, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 4);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 5, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 6);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 7, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 8);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 9, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 10);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 11, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 12);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 13, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 14);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 15, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 16);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 17, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 18);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 19, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 20);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 21, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 200, 0, 22);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (0, 23, 2219223.0599095495, 43261.330613416343, 0, 780, 0, 50, 0, 24);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (1, 2, 2201674.04542425, -537.96920408041944, 0, 780, 0, -50, 1, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (2, 4, 2220804.1661061305, -1824.0976658091467, 0, 780, 0, -50, 3, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (3, 6, 2170182.7454079189, -42278.887298092624, 0, 780, 0, -50, 5, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (4, 8, 2214706.3217116385, 42201.317464497493, 0, 780, 0, -50, 7, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (5, 10, 2221020.0081376741, 13156.086760460585, 0, 780, 0, -50, 9, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (6, 12, 2276566.54710769, -10390.65374555897, 0, 780, 0, -50, 11, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (7, 14, 2273317.2298506126, 45902.567392510609, 0, 780, 0, -50, 13, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (8, 16, 2268126.8554679737, 4674.1135220944607, 0, 780, 0, -50, 15, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (9, 18, 2200293.6413550111, -57036.325719941706, 0, 780, 0, -50, 17, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (10, 20, 2179348.5807632855, 7268.5531586333609, 0, 780, 0, -50, 19, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (11, 22, 2226924.5635206043, 27207.476487862386, 0, 780, 0, -200, 21, 0);
INSERT INTO vrpdtw_beer (rid, id, x, y, opentime, closetime, servicetime, demand, pindex, dindex) VALUES (12, 24, 2214670.8347013257, -18087.138344661518, 0, 780, 0, -50, 23, 0);
*/

/*
SELECT * FROM pgr_gsoc_vrppdtw(
    $$ SELECT * FROM jet_customers $$,
    2, 5);

SELECT * FROM _pgr_pickDeliver(
    $$ SELECT * FROM jet_customers $$,
    2, 5);

SELECT * FROM _pgr_pickDeliver(
    $$ SELECT * FROM jet_customers $$,
    max_vehicles := 2, capacity :=5, speed := 1);
*/

/*
-- converting data to use euclidean
UPDATE jet_customers SET opentime = 9*60 WHERE opentime = 3448;
UPDATE jet_customers SET opentime = 9.5*60 WHERE opentime = 3463;
UPDATE jet_customers SET opentime = 10*60 WHERE opentime = 3576;
UPDATE jet_customers SET opentime = 10.5*60 WHERE opentime = 3624;
UPDATE jet_customers SET opentime = 11*60 WHERE opentime = 4265;
UPDATE jet_customers SET opentime = 11.5*60 WHERE opentime = 4355;

UPDATE jet_customers SET closetime = 11*60 WHERE closetime = 2853448;
UPDATE jet_customers SET closetime = 11.5*60 WHERE closetime = 2853463;
UPDATE jet_customers SET closetime = 12*60 WHERE closetime = 2853576;
UPDATE jet_customers SET closetime = 12.5*60 WHERE closetime = 2853624;
UPDATE jet_customers SET closetime = 13*60 WHERE closetime = 2854265;
UPDATE jet_customers SET closetime = 13.5*60 WHERE closetime = 2854355;
UPDATE jet_customers SET closetime = 14*60 WHERE closetime = 6903448;
UPDATE jet_customers SET closetime = 14.5*60 WHERE closetime = 6903463;
UPDATE jet_customers SET closetime = 15*60 WHERE closetime = 6903576;
UPDATE jet_customers SET closetime = 15.5*60 WHERE closetime = 6903624;
UPDATE jet_customers SET closetime = 16*60 WHERE closetime = 6904265;
UPDATE jet_customers SET closetime = 16.5*60 WHERE closetime = 6904355;

UPDATE jet_customers SET servicetime = 2 WHERE servicetime != 0;

WITH
customer_data AS (SELECT * FROM jet_customers),
pickups AS (
    SELECT id, demand, x as pick_x, y as pick_y, opentime as pick_open, closetime as pick_close, servicetime as pick_service
    FROM  customer_data WHERE pindex = 0 AND id != 0
),
deliveries AS (
    SELECT pindex AS id, x as deliver_x, y as deliver_y, opentime as deliver_open, closetime as deliver_close, servicetime as deliver_service
    FROM  customer_data WHERE dindex = 0 AND id != 0
)
SELECT * INTO jet_orders FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id;

WITH
customer_data AS (select * from jet_customers)
SELECT id, x AS start_x, y AS start_y,
opentime AS start_open, closetime AS start_close,
2 AS capacity,  5  AS number INTO jet_vehicles
FROM customer_data WHERE id = 0 LIMIT 1;

SELECT *
FROM _pgr_pickDeliverEuclidean(
    $$ SELECT * from jet_orders $$,
    $$ SELECT * FROM jet_vehicles$$
);
*/


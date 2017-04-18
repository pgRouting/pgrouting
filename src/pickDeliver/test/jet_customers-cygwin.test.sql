
CREATE TABLE jet_customers (
    id integer,
    x integer,
    y integer,
    pindex integer,
    dindex integer,
    demand integer,
    opentime double precision,
    closetime double precision,
    servicetime double precision
);

CREATE TABLE stops (
    iata_faa character varying(20),
    x integer,
    y integer,
    longitude double precision,
    latitude double precision
);

--
-- TOC entry 4256 (class 0 OID 6648805)
-- Dependencies: 300
-- Data for Name: jet_customers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY jet_customers (id, x, y, pindex, dindex, demand, opentime, closetime, servicetime) FROM stdin;
1	1644	-614	0	2	7	0	60000	30
3	1644	-614	0	4	17	0	60000	30
5	1644	-614	0	6	10	0	60000	30
7	1644	-614	0	8	5	0	60000	30
9	2320	126	0	10	13	0	60000	30
11	2320	126	0	12	12	0	60000	30
13	2320	126	0	14	9	0	60000	30
15	2320	126	0	16	2	0	60000	30
17	2035	-176	0	18	6	0	60000	30
19	2035	-176	0	20	16	0	60000	30
21	2035	-176	0	22	6	0	60000	30
23	2383	448	0	24	14	0	60000	30
25	2383	448	0	26	20	0	60000	30
27	2383	448	0	28	6	0	60000	30
2	2383	448	1	0	-7	0	60000	30
4	1981	-1903	3	0	-17	0	60000	30
6	2035	-176	5	0	-10	0	60000	30
8	2387	34	7	0	-5	0	60000	30
10	2383	448	9	0	-13	0	60000	30
12	1981	-1903	11	0	-12	0	60000	30
14	2035	-176	13	0	-9	0	60000	30
16	2387	34	15	0	-2	0	60000	30
18	2383	448	17	0	-6	0	60000	30
20	1981	-1903	19	0	-16	0	60000	30
22	2387	34	21	0	-6	0	60000	30
24	1981	-1903	23	0	-14	0	60000	30
26	2035	-176	25	0	-20	0	60000	30
28	2387	34	27	0	-6	0	60000	30
0	2138	-119	0	0	0	0	60000	0
\.


--
-- TOC entry 4255 (class 0 OID 6648802)
-- Dependencies: 299
-- Data for Name: stops; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY stops (iata_faa, x, y, longitude, latitude) FROM stdin;
BOS	2320	126	-71.005180999999993	42.364347000000002
BGR	2383	448	-68.828138999999993	44.807443999999997
MIA	1981	-1903	-80.290555999999995	25.79325
TEB	2138	-119	-74.060837000000006	40.850102999999997
MVY	2387	34	-70.615278000000004	41.391666999999998
ABE	2035	-176	-75.440805999999995	40.652082999999998
BKW	1644	-614	-81.124200000000002	37.787300000000002
BGE	1466	-1424	-84.636927799999995	30.971598100000001
\.

-- crashes backend
WITH
pickups AS (
    SELECT id, demand, x as p_x, y as p_y, opentime as p_open, closetime as p_close, servicetime as p_service
    FROM  jet_customers WHERE pindex = 0 AND id != 0
),
deliveries AS (
    SELECT pindex AS id, x as d_x, y as d_y, opentime as d_open, closetime as d_close, servicetime as d_service
    FROM  jet_customers WHERE dindex = 0 AND id != 0
)
SELECT * INTO jet_orders
FROM pickups JOIN deliveries USING(id) ORDER BY pickups.id;


SELECT * FROM pgr_gsoc_vrppdtw(
    'SELECT id, x, y, demand, opentime, closetime, servicetime, pindex, dindex FROM jet_customers ORDER BY id', 20, 100);

SELECT * FROM _pgr_pickDeliver(
    'SELECT id, x, y, demand, opentime, closetime, servicetime, pindex, dindex FROM jet_customers ORDER BY id', max_vehicles := 20, capacity:= 100, speed := 1000);

SELECT * FROM _pgr_pickDeliverEuclidean(
    'SELECT * FROM jet_orders ORDER BY id',
    'SELECT 0 AS id,
    2138 AS start_x, -119 AS start_y,
    0 AS start_open, 60000 AS start_close,
    100 AS capacity, 20 AS number, 1000 as speed',
    30);

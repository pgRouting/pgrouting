--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: edges1; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE edges1 (
    eid integer NOT NULL,
    dir character varying,
    source integer,
    target integer,
    cost double precision,
    reverse_cost double precision,
    x1 double precision,
    y1 double precision,
    x2 double precision,
    y2 double precision,
    to_cost double precision,
    rule text,
    the_geom geometry,
    CONSTRAINT enforce_dims_the_geom CHECK ((st_ndims(the_geom) = 2)),
    CONSTRAINT enforce_geotype_the_geom CHECK (((geometrytype(the_geom) = 'LINESTRING'::text) OR (the_geom IS NULL))),
    CONSTRAINT enforce_srid_the_geom CHECK ((st_srid(the_geom) = (-1)))
);


--
-- Name: restrictions1; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE restrictions1 (
    rid integer NOT NULL,
    to_cost double precision,
    teid integer,
    feid integer,
    via text
);


--
-- Name: restrictions1_rid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE restrictions1_rid_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- Name: restrictions1_rid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE restrictions1_rid_seq OWNED BY restrictions1.rid;


--
-- Name: restrictions1_rid_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('restrictions1_rid_seq', 1, true);


--
-- Name: restrictions1a; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE restrictions1a (
    rid integer,
    to_cost double precision,
    teid integer,
    feid integer,
    via text
);


--
-- Name: rid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE restrictions1 ALTER COLUMN rid SET DEFAULT nextval('restrictions1_rid_seq'::regclass);


--
-- Data for Name: edges1; Type: TABLE DATA; Schema: public; Owner: -
--

COPY edges1 (eid, dir, source, target, cost, reverse_cost, x1, y1, x2, y2, to_cost, rule, the_geom) FROM stdin;
1	B	1	2	1	1	2	0	2	1	\N	\N	010200000002000000000000000000004000000000000000000000000000000040000000000000F03F
2	TF	2	3	-1	1	2	1	3	1	\N	\N	0102000000020000000000000000000040000000000000F03F0000000000000840000000000000F03F
3	TF	3	4	-1	1	3	1	4	1	\N	\N	0102000000020000000000000000000840000000000000F03F0000000000001040000000000000F03F
4	B	2	7	1	1	2	1	2	2	\N	\N	0102000000020000000000000000000040000000000000F03F00000000000000400000000000000040
5	FT	3	8	1	-1	3	1	3	2	\N	\N	0102000000020000000000000000000840000000000000F03F00000000000008400000000000000040
6	B	5	6	1	1	0	2	1	2	\N	\N	01020000000200000000000000000000000000000000000040000000000000F03F0000000000000040
7	B	6	7	1	1	1	2	2	2	\N	\N	010200000002000000000000000000F03F000000000000004000000000000000400000000000000040
8	B	7	8	1	1	2	2	3	2	\N	\N	0102000000020000000000000000000040000000000000004000000000000008400000000000000040
9	B	8	9	1	1	3	2	4	2	\N	\N	0102000000020000000000000000000840000000000000004000000000000010400000000000000040
10	B	7	10	1	1	2	2	2	3	\N	\N	0102000000020000000000000000000040000000000000004000000000000000400000000000000840
11	FT	8	11	1	-1	3	2	3	3	\N	\N	0102000000020000000000000000000840000000000000004000000000000008400000000000000840
12	FT	10	11	1	-1	2	3	3	3	\N	\N	0102000000020000000000000000000040000000000000084000000000000008400000000000000840
13	FT	11	12	1	-1	3	3	4	3	\N	\N	0102000000020000000000000000000840000000000000084000000000000010400000000000000840
14	B	10	13	1	1	2	3	2	4	\N	\N	0102000000020000000000000000000040000000000000084000000000000000400000000000001040
15	B	9	12	1	1	4	2	4	3	\N	\N	0102000000020000000000000000001040000000000000004000000000000010400000000000000840
16	B	4	9	1	1	4	1	4	2	\N	\N	0102000000020000000000000000001040000000000000F03F00000000000010400000000000000040
\.


--
-- Data for Name: restrictions1; Type: TABLE DATA; Schema: public; Owner: -
--

COPY restrictions1 (rid, to_cost, teid, feid, via) FROM stdin;
1	100	7	4	\N
2	4	8	3	5
3	100	9	16	\N
\.


--
-- Data for Name: restrictions1a; Type: TABLE DATA; Schema: public; Owner: -
--

COPY restrictions1a (rid, to_cost, teid, feid, via) FROM stdin;
1	100	7	4	\N
\.


--
-- Name: edges1_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY edges1
    ADD CONSTRAINT edges1_pkey PRIMARY KEY (eid);


--
-- Name: restrictions1_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY restrictions1
    ADD CONSTRAINT restrictions1_pkey PRIMARY KEY (rid);


--
-- PostgreSQL database dump complete
--
/*

select * from turn_restrict_shortest_path(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,     -- edge_id for start
    0.5,   -- midpoint of edge
    6,     -- edge_id of route end
    0.5,   -- midpoint of edge
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    null); -- no turn restrictions

select * from turn_restrict_shortest_path(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,     -- node_id of start
    5,     -- node_id of end
    true,  -- directed graph?
    true,  -- has_reverse_cost?
    null); -- no turn restrictions

select * from turn_restrict_shortest_path(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,    -- edge_id for start
    0.5,  -- midpoint of edge
    6,    -- edge_id of route end
    0.5,  -- midpoint of edge
    true, -- directed graph?
    true, -- has_reverse_cost?
              -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');

select * from turn_restrict_shortest_path(
    'select eid as id, source::integer, target::integer,cost, reverse_cost from edges1',
    1,     -- node_id of start
    5,     -- node_id of end
    true,  -- directed graph?
    true,  -- has_reverse_cost?
               -- include the turn restrictions
    'select to_cost, teid as target_id, feid||coalesce('',''||via,'''') as via_path from restrictions1');

*/


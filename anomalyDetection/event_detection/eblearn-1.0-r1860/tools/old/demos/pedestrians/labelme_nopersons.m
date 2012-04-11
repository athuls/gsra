%% Labelme extract images without people
%% by Pierre Sermanet, pierre.sermanet@gmail.com, 2010

%% Setting paths
experiment = 'nofaces_static' % name of experiment and output directory
root = '~/humairadata/labelme/'
outroot = sprintf('%s/out/%s', root, experiment);
p = path;
path(p, '~/labelme/LabelMeToolbox/');
HOMEIMAGES = sprintf('%s/Images', root);
HOMEANNOTATIONS = sprintf('%s/Annotations/', root);
dbfile = 'full_database.mat';

%% Download entire dataset locally
% LMinstall (HOMEIMAGES, HOMEANNOTATIONS);

%% Create index file from xml files
%D = LMdatabase(HOMEANNOTATIONS);
% save database into a file
%fprintf('saving full database (%d elements) to file...', length(D));
%save(dbfile, 'D');

%% extract images of interest
% load database file
%fprintf('loading database file %s...', dbfile);
%load(dbfile, '-mat');
%fprintf('loaded database');
% D
% remove sequences?
static = LMquery(D, 'folder', 'static');

% LMdbshowscenes(D(1:5), HOMEIMAGES); % shows the first 30 images

%D = LMdatabase(HOMEANNOTATIONS);
DB = static;
%% query images with no faces or persons
fprintf('querying...');
[all,jall] = LMquery(DB, 'object.name', '-toto');
[humans,jhumans] = LMquery(DB, 'object.name', 'head,face,person,human,man,woman,crowd,cycliste,cyclist,pedestrian');
jnoface = setdiff(jall, jhumans);
noface = DB(jnoface);

%% save queried database into experiment folder
destannots = sprintf('%s/Annotations/', outroot);
destimages = sprintf('%s/Images/', outroot);
fprintf('saving queried images (%d images) into %s', length(noface), outroot);
LMwritedatabase(noface, HOMEIMAGES, destimages, destannots)
fprintf('saving done.');

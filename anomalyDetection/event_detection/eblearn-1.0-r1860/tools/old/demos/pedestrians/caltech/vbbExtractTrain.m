% add a directory link to the caltech code directory
% add your caltech code path
% addpath('/home/sermanet/matlab/caltech_pedestrians')

% inria extraction for daimler-similar data, i.e. 96x48 with same context size
% vbbExtract('inria-train', 0, .65, 96, 1.3, 1.3 * 1.16, .2, 1)

function vbbExtract(name, display, minoverlap, minheight, hcontext, wcontext, ...
		    minaratio, extractBg)
% Extract all pedestrian images and non-pedestrian images
% by Pierre Sermanet
% INPUTS
%   display - if 1, display which windows are extracted.
%   minoverlap - ignore windows with lower overlap of visible/predicted
%                accepted occlusion categories are:
%                .65 -> accepting partial occlusion
%                .2  -> accepting heavy occlusion
%                0   -> accepting full occlusion
%   minheight - ignore windows with lower height than minheight
%                height categories are:
%                >= 80 -> near
%                >= 30 -> medium
%                >= 0   -> far
%   {h,w}context - a factor applied to bbox height and width, used to increase
%               context. e.g. factor 1.2 will result in bbox of size
%               height*1.2 x width*1.2 and bbox origin moved accordingly.
%   minaratio - minimum aspect ratio, ignore ratio lower than this
%               This will get rid of some false bounding boxes like half
%               (or less) pedestrian at edge of image.
%   extractBg - modulo at which to extract background images (pedestrian
%               free). e.g. 1 will extract all backgrounds, 30 will
%               extract background every 30 frames.
%  figure
  % default values
  if (nargin < 7) extractBg = 60; end
  if (nargin < 6) minaratio = .2; end
  if (nargin < 5) wcontext = 1.3; end
  if (nargin < 5) hcontext = 1.3; end
  if (nargin < 4) minheight = 30; end
  if (nargin < 3) minoverlap = .65; end
  if (nargin < 2) display = 0; end
  if (nargin < 1) name = 'use-all'; end

  [pth,setIds,nVids] = dbInfo(name);
  extracted = sprintf('extracted_minoverlap%0.2f_minheight%i_hcontext%0.1f_wcontext%0.1f', ...
		      minoverlap, minheight, hcontext, wcontext);
  outdir_ped = [pth '/' extracted '/ped/'];
  outdir_bg = [pth '/' extracted '/bg/'];
  for i=1:length(setIds)
    setId = uint32(setIds(i)) + 1;
    nVid = uint32(nVids(setId));
    for vidId=1:nVid
      % load annotations
      vStr = sprintf('set%02i/V%03i', setId-1, vidId-1);
      fprintf('extracting from video file %s\n', vStr);
      A = vbb('vbbLoad', [pth '/annotations/' vStr] );
      % open IO interface for this video
      seqname = [pth '/videos/' vStr];
      sr = seqIo(seqname, 'reader');
      fprintf('%s contains %i annotated frames\n', seqname, A.nFrame);
      % create directories
      fname_ped = [outdir_ped vStr];
      if (~exist(fname_ped))
	mkdir(fname_ped);
      end
      if (extractBg > 0)
	fname_bg = [outdir_bg vStr];
	if (~exist(fname_bg))
	  mkdir(fname_bg);
	end
      end
      % process all frames
      for iframe=1:A.nFrame
	fprintf('  frame %i\n', iframe);
	% get frame
	sr.seek(iframe - 1);
	I = sr.getframe();
	% display original annotations
	if (display == 1)
	  subplot(1, 3, 1), imshow(I, 'Border', 'tight');
	  vbb('drawToFrame', A, iframe);
	  title('original annotations');
	end
	% extract pedestrians
	savePeds(I, A, iframe, fname_ped, display, minoverlap, minheight, ...
		 minaratio, hcontext, wcontext);
	% extract non pedestrians
	if (mod(iframe, extractBg) == 0)
	  saveBg(I, A, iframe, fname_bg, display);
	end
      end
    end
  end
end

function savePeds(I, A, iframe, fname, display, minoverlap, minheight, ...
		  minaratio, hcontext, wcontext)
% Save all pedestrian images for this frame
  if (display == 1)
    subplot(1, 3, 2), imshow(I, 'Border', 'tight');
    title('extracted pedestrians');
  end
  peds = 0;
  i = 0;
  for o=A.objLists{iframe}
    % global bounding box
    pos = o.pos;
    % bring indices to matlab's style
    if (pos(1) < 0) pos(1) = 0; end
    if (pos(2) < 0) pos(2) = 0; end
    pos(1) = pos(1) + 1;
    pos(2) = pos(2) + 1;
    if (pos(3) == 0) || (pos(4) == 0)
      continue % ignore empty boxes
    end
    if (strcmp(A.objLbl{o.id}, 'people'))
      continue % ignore people
    end
    if (strcmp(A.objLbl{o.id}, 'person?'))
      continue % ignore person?
    end
    if (~strcmp(A.objLbl{o.id}, 'person'))
      continue % ignore anything not 'person'
    end
    if (pos(4) < minheight)
      fprintf('    obj %i: ignoring height %i (smaller than minheight %i)\n', ...
	      o.id, pos(4), minheight);
      continue % ignore smaller height than minheight
    end
    % ignore when aspect ratio is lower than minaratio
    ratio = pos(3) / pos(4);
    if (pos(3) / pos(4) < minaratio)
      fprintf('    obj %i: ignoring aspect ratio %i (smaller than minaratio %i)\n',...
	      o.id, ratio, minaratio);
      continue
    end
    % ignore when visible part is smaller than threshold
    if (~(all(o.posv) == 0)) % a predicted exists
      % overlap between predicted and visible
      overlap = bbGt('compOa', o.pos, o.posv, 0);
      if (overlap < minoverlap)
	fprintf('    obj %i: ignoring overlap %i\n', o.id, overlap);
	continue
      else
	fprintf('    obj %i: accepting overlap %i\n', o.id, overlap);
      end
    end
    % add context 
    pos(1) = pos(1) - ((pos(3) * wcontext - pos(3)) / 2);
    pos(2) = pos(2) - ((pos(4) * hcontext - pos(4)) / 2);
    pos(3) = pos(3) * wcontext;
    pos(4) = pos(4) * hcontext;
    % ignore if box is overlapping outside image
    if ((uint32(pos(1) + pos(3)) > size(I, 2)) || ...
	(uint32(pos(2) + pos(4)) > size(I, 1)) || ...
	(uint32(pos(1)) < 1) || (uint32(pos(2)) < 1))
      continue
    end
    peds = 1; % we found humans
    if (display == 1)
      bbApply('draw', pos, 'g', 2, '-');
    end
    fprintf('    obj %i: cropping with %i %i %i %i: \n', o.id, ...
	    uint32(pos(1)), uint32(pos(2)), uint32(pos(3)), uint32(pos(4)));
    % crop image
    IC = imcrop(I, pos);
    % save image
    fn = sprintf('%s/frame%i_obj%i.ppm', fname, iframe, i);
    imwrite(IC, fn, 'ppm');
    fprintf('    obj %i: saved %s\n', o.id, fn);
    i = i + 1;
  end
  if (display == 1) && (peds == 1)
    pause(1);
  end
end

function saveBg(I, A, iframe, fname, display)
% Save image after blanking all pedestrian without restriction
  for o=A.objLists{iframe}
    % global bounding box
    pos = o.pos;
    % bring indices to matlab's style
    if (pos(1) < 0) pos(1) = 0; end
    if (pos(2) < 0) pos(2) = 0; end
    pos(1) = pos(1) + 1;
    pos(2) = pos(2) + 1;
    if (uint32(pos(1)) == 0)
      fprintf('toto\n');
    end
    if (pos(3) == 0) || (pos(4) == 0)
      continue % ignore empty boxes
    end
    fprintf('    obj %i: clearing %i %i %i %i: \n', o.id, ...
	    uint32(pos(1)), uint32(pos(2)), uint32(pos(3)), uint32(pos(4)));
    % clear bbox
    range1 = [ uint32(pos(1)) : uint32(pos(1) + pos(3)) ];
    range2 = [ uint32(pos(2)) : uint32(pos(2) + pos(4)) ];
    range3 = [1, 2, 3]; 
    I(range2, range1, range3) = 0;
  end
  if (display == 1)
    subplot(1, 3, 3), imshow(I, 'Border', 'tight');
    title('pedestrian-free');
  end
  % save image
  fn = sprintf('%s/frame%i.ppm', fname, iframe);
  imwrite(I, fn, 'ppm');
  fprintf('    bg: saved %s\n', fn);
end

function make_graph(directory,file,xLabel,yLabel,makeRelativeGraph,relativeYLabel)

    if ~exist(['data/' directory '/CL/' file])
        return
    end

    % if(this is the 1D problem's plot)
    if(length(strfind(pwd,"2D")) == 0)
        twoD = ''
        theoryValue = 0.3267
    else % else it must be the 2D version
        twoD = '_2D'
        theoryValue = 0.3187
    end

    % Plotting constants
    LINE_WIDTH = 5;
    FONT_SIZE = 16;

    % Load the data into memory
    CL=load(['data/' directory '/CL/' file]);
    nonCL=load(['data/' directory '/nonCL/' file]);
    
    % Create a new plot window to draw on; and plot x, y, and deltaY
    % for both curriculum learning and non-curriculum learning values
    handle=figure();
    eb = errorbar(CL(:,1),CL(:,2),CL(:,3),'o');
    set(eb,'LineWidth',LINE_WIDTH);
    hold on
    eb = errorbar(nonCL(:,1),nonCL(:,2),nonCL(:,3),'rx');
    set(eb,'LineWidth',LINE_WIDTH);

    % Adjust the axis to fit the data tightly
    axis([CL(1,1) CL(end,1)])

    % Set the axis labels
    xlabel(xLabel,'FontSize',FONT_SIZE)
    ylabel(yLabel,'FontSize',FONT_SIZE)
    set(gca,'FontSize',FONT_SIZE)

    % Save the plot to disk
    saveas(handle,['graphs/' file twoD],'png');

    %%%%%%%%%%%%%%%%%%%%%%%
    % Relative Difference %
    %%%%%%%%%%%%%%%%%%%%%%%
    if(makeRelativeGraph)
        x  = CL(:,1);
        x = x(2:end);
        y1 = CL(:,2);
        error1 = CL(:,3);
        y2 = nonCL(:,2);
        error2 = nonCL(:,3);

        diff = y1-y2;
        diffError = sqrt(error1.^2 + error2.^2);

        relDiff = abs(diff ./ y2);
        relDiffError = relDiff .* sqrt( (diffError ./ diff).^2 + (error2 ./ y2).^2);

        relDiff = relDiff(2:end); %-- Avoid divide by zero in the first element
        relDiffError = relDiffError(2:end);

        handle=figure();
        eb = errorbar(x,relDiff,relDiffError,'x');
        set(eb,'LineWidth',LINE_WIDTH);
        hold on

        plot([x(1) x(end)],[theoryValue theoryValue],'r','LineWidth',LINE_WIDTH)
        xlim([x(1) x(end)])
        ylim([0.2 0.45])

        xlabel(xLabel,'FontSize',FONT_SIZE)
        ylabel(relativeYLabel,'FontSize',FONT_SIZE)
        set(gca,'FontSize',FONT_SIZE)

        % Save the plot to disk
        saveas(handle,['graphs/relative_' file twoD],'png');
        end
    end

mkdir graphs;
make_graph('RvT', 'RvT', 'Training Iterations', 'Testing Reward', false);
make_graph('TvG', 'TvG', 'Games', 'Time Taken (Iterations)', true, 'Relative Time Difference');
make_graph('QvT', 'Q_0', 'Iterations', 'Q-value for Action "Move Left"', false);
make_graph('QvT', 'Q_1', 'Iterations', 'Q-value for Action "Move Right"',false);

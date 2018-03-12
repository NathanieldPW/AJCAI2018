function t = calculate_t(P,NUM_TRANSIENT_STATES,NUM_STATES)
    Q = P(1:NUM_TRANSIENT_STATES,1:NUM_TRANSIENT_STATES);
    R = P(1:NUM_TRANSIENT_STATES,NUM_TRANSIENT_STATES+1:NUM_STATES);
    %I = P(NUM_TRANSIENT_STATES+1:NUM_STATES,NUM_TRANSIENT_STATES+1:NUM_STATES)

    I = eye(length(Q));
    N = inv(I - Q);

    t       = N*ones(length(N),1);
end

function gridWorld1D()
    display("---- Grid World 1D ----")

    NUM_STATES           = 101;
    NUM_ABSORBING_STATES = 2;
    NUM_TRANSIENT_STATES = NUM_STATES-NUM_ABSORBING_STATES;

    PROB_RIGHT = 0.5;
    PROB_LEFT  = 0.5;

    P=zeros(NUM_STATES);

    % Probability of going to the right
    for i = 1:NUM_TRANSIENT_STATES
        P(i,i+1) = PROB_RIGHT;
    end

    % Probability of going to the left
    for i = 2:NUM_TRANSIENT_STATES
        P(i,i-1) = PROB_LEFT;
    end

    % The states are labelled such that the left-most absorbing state has the
    % highest index. So, going left from position 1 takes you to position 101,
    % which is absorbing. Going right from 99 puts into the other absorbing state
    % labelled 100. Basically, going left from the left-most index wraps around.
    P(1,NUM_STATES) = PROB_LEFT;
    %P(1,2)          = PROB_RIGHT;

    % Probabilities of absorbing states
    for i = NUM_TRANSIENT_STATES+1:NUM_STATES
        P(i,i) = 1;
    end

    t       = calculate_t(P,NUM_TRANSIENT_STATES,NUM_STATES);
    tMiddle = ceil(length(t)/2);

    nonCL   = t(tMiddle)
    CL      = mean(t)
    ratio   = 1 - (CL/nonCL)

    % The rejected paper had mistakes in the state labelling and the theory math.
    % The states are to be labelled 0->N, INCLUDING the absorbing states. Which
    % means the formula is supposed to use N=100, not N=101.
end

function gridWorld2D()
    display("---- Grid World 2D ----")

    NUM_STATES           = 18;
    NUM_ABSORBING_STATES = 1;
    NUM_TRANSIENT_STATES = NUM_STATES-NUM_ABSORBING_STATES;

    PROB_UP    = 0.25;
    PROB_DOWN  = 0.25;
    PROB_RIGHT = 0.25;
    PROB_LEFT  = 0.25;

    P=zeros(NUM_STATES);

    % _W_WG     16  - 17  - 18
    % ___W_     12 13 14  - 15
    % _W___  =>  8  -  9 10 11   
    % __WWW      6  7  -  -  -
    % _____      1  2  3  4  5

    P(1,2) = PROB_RIGHT;
    P(1,6) = PROB_UP;
    P(1,1) = 1 - sum(P(1,:));

    P(2,1) = PROB_LEFT;
    P(2,3) = PROB_RIGHT;
    P(2,7) = PROB_UP;
    P(2,2) = 1 - sum(P(2,:));

    P(3,2) = PROB_LEFT;
    P(3,4) = PROB_RIGHT;
    P(3,3) = 1 - sum(P(3,:));

    P(4,3) = PROB_LEFT;
    P(4,5) = PROB_RIGHT;
    P(4,4) = 1 - sum(P(4,:));

    P(5,4) = PROB_LEFT;
    P(5,5) = 1 - sum(P(5,:));

    % _W_WG     16  - 17  - 18
    % ___W_     12 13 14  - 15
    % _W___  =>  8  -  9 10 11   
    % __WWW      6  7  -  -  -
    % _____      1  2  3  4  5

    P(6,8) = PROB_UP;
    P(6,7) = PROB_RIGHT;
    P(6,1) = PROB_DOWN;
    P(6,6) = 1 - sum(P(6,:));

    P(7,6) = PROB_LEFT;
    P(7,2) = PROB_DOWN;
    P(7,7) = 1 - sum(P(7,:));

    P(8,12) = PROB_UP;
    P(8,6)  = PROB_DOWN;
    P(8,8)  = 1 - sum(P(8,:));

    P(9,14) = PROB_UP;
    P(9,10) = PROB_RIGHT;
    P(9,9)  = 1 - sum(P(9,:));

    P(10,9)  = PROB_LEFT;
    P(10,11) = PROB_RIGHT;
    P(10,10) = 1 - sum(P(10,:));

    % _W_WG     16  - 17  - 18
    % ___W_     12 13 14  - 15
    % _W___  =>  8  -  9 10 11   
    % __WWW      6  7  -  -  -
    % _____      1  2  3  4  5

    P(11,10) = PROB_LEFT;
    P(11,15) = PROB_UP;
    P(11,11) = 1 - sum(P(11,:));

    P(12,16) = PROB_UP;
    P(12,13) = PROB_RIGHT;
    P(12,8)  = PROB_DOWN;
    P(12,12) = 1 - sum(P(12,:));

    P(13,12) = PROB_LEFT;
    P(13,14) = PROB_RIGHT;
    P(13,13) = 1 - sum(P(13,:));

    P(14,13) = PROB_LEFT;
    P(14,17) = PROB_UP;
    P(14,9)  = PROB_DOWN;
    P(14,14) = 1 - sum(P(14,:));

    P(15,18) = PROB_UP;
    P(15,11) = PROB_DOWN;
    P(15,15) = 1 - sum(P(15,:));

    % _W_WG     16  - 17  - 18
    % ___W_     12 13 14  - 15
    % _W___  =>  8  -  9 10 11   
    % __WWW      6  7  -  -  -
    % _____      1  2  3  4  5

    P(16,12) = PROB_DOWN;
    P(16,16) = 1 - sum(P(16,:));

    P(17,14) = PROB_DOWN;
    P(17,17) = 1 - sum(P(17,:));

    % Absorbing goal state
    P(18,18) = 1;

    t       = calculate_t(P,NUM_TRANSIENT_STATES,NUM_STATES);

    nonCL   = t(1)
    CL      = mean([t(1),t(13),t(11)])
    ratio   = 1 - (CL/nonCL)
end

gridWorld1D()
gridWorld2D()
